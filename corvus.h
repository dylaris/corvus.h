/*
 * NOTE: Macros are used extensively. Beware of side effects.
 *
 * configuration macro
 * - CORVUSDEF
 * - CORVUS_INITCAP
 * - CORVUS_REALLOC
 * - CORVUS_FREE
 * - CORVUS_ASSERT
 *
 * flag macro
 * - CORVUS_NO_LOG
 * - CORVUS_IMPLEMENTATION
 */

#ifndef CORVUS_H
#define CORVUS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>

#ifndef CORVUSDEF
#define CORVUSDEF
#endif

#ifndef CORVUS_INITCAP
#define CORVUS_INITCAP 128
#endif

#ifndef CORVUS_REALLOC
#define CORVUS_REALLOC realloc
#endif

#ifndef CORVUS_FREE
#define CORVUS_FREE free
#endif

#ifndef CORVUS_ASSERT
#define CORVUS_ASSERT assert
#endif

/* dynamic array */

typedef struct {
    int len;
    int cap;
} ArrayHeader;

#define Array(T) T *
#define arrhdr(a) ((ArrayHeader*)((char*)a-sizeof(ArrayHeader)))
#define arrlen(a) ((a) ? arrhdr(a)->len : 0)
#define arrcap(a) ((a) ? arrhdr(a)->cap : 0)
/* Note: after calling realloc, the old a becomes invalid.
   Always capture the old state (capacity, etc.) before
   reallocation, and never use the old a afterwards. */
#define arrkeep(a, sz) do { \
    if (arrcap(a) < (sz)) { \
        int newcap = (sz); \
        int elemsz = sizeof(*(a)); \
        bool init = (a) == NULL; \
        ArrayHeader *h = init ? NULL : arrhdr(a); \
        h = CORVUS_REALLOC(h, sizeof(ArrayHeader)+newcap*elemsz); \
        CORVUS_ASSERT(h && "run out of memory"); \
        if (init) h->len = 0; \
        h->cap = newcap; \
        (a) = (void *) (h + 1); \
    } \
} while (0)
#define arrpush(a, e) do { \
    if (arrcap(a) == 0) { \
        arrkeep(a, CORVUS_INITCAP); \
    } else if (arrlen(a) + 1 > arrcap(a)) { \
        arrkeep(a, 2 * arrcap(a)); \
    } \
    (a)[arrhdr(a)->len++] = (e); \
} while (0)
#define arrpop(a) (CORVUS_ASSERT(arrlen(a)>0), (a)[--arrhdr(a)->len])
#define arrshift(a, e) do { \
    if (arrlen(a) == 0) { \
        arrpush(a, e); \
    } else { \
        if (arrlen(a) + 1 > arrcap(a)) { \
            arrkeep(a, 2 * arrcap(a)); \
        } \
        memmove((a)+1, (a), arrlen(a)*sizeof(*(a))); \
        (a)[0] = (e); \
        arrhdr(a)->len++; \
    } \
} while (0)
#define arrunshift(a) do { \
    CORVUS_ASSERT(arrlen(a)>0); \
    memmove((a), (a)+1, (--arrhdr(a)->len)*sizeof(*(a))); \
} while (0)
#define arrtop(a) (CORVUS_ASSERT(a), (a)[arrlen(a)-1])
#define arrbot(a) (CORVUS_ASSERT(a), (a)[0])
#define arrrev(T, a) do { \
    for (int i = 0, j = arrlen(a) - 1; i < j; i++, j--) { \
        swap(T, (a)[i], (a)[j]); \
    } \
} while (0)
#define arrclear(a) do { CORVUS_ASSERT(a); arrhdr(a)->len = 0; } while (0)
#define arrfree(a) do { CORVUS_ASSERT(a); CORVUS_FREE(arrhdr(a)); } while (0)
#define arrbegin(a) (CORVUS_ASSERT(a), a)
#define arrend(a) (CORVUS_ASSERT(a), (a)+arrlen(a))
#define arrforeach(T, a) for (T *it = arrbegin(a); it < arrend(a); it++)

/* string view & string buffer */

typedef struct {
    const char *ptr;
    int len;
} StrView;

typedef struct {
    char *chars;
    int len;
    int cap;
} StrBuf;

CORVUSDEF StrView svfromcsr(const char *cstr);
CORVUSDEF StrView svfromsbuf(const StrBuf sb);
CORVUSDEF bool sveq(StrView a, StrView b);
CORVUSDEF bool sveqs(StrView sv, const char *cstr);
CORVUSDEF StrView svsub(StrView sv, int start, int len);
CORVUSDEF int svchr(StrView sv, char c);
CORVUSDEF int svstr(StrView sv, StrView sub);
CORVUSDEF StrView svtrim(StrView sv);
CORVUSDEF StrView svtriml(StrView sv);
CORVUSDEF StrView svtrimr(StrView sv);
CORVUSDEF StrView svchop(StrView sv, char c);
CORVUSDEF bool svstart(StrView sv, StrView prefix);
CORVUSDEF bool svend(StrView sv, StrView suffix);
CORVUSDEF char *svtocstr(const StrView sv);
#define svforeach(sv) for (const char *it = (sv).ptr; it < (sb).ptr + (sv).len; it++)

CORVUSDEF void sbclear(StrBuf *sb);
CORVUSDEF void sbfree(StrBuf *sb);
CORVUSDEF void sbcats(StrBuf *sb, const char *cstr);
CORVUSDEF void sbcatc(StrBuf *sb, char c);
CORVUSDEF void sbcatf(StrBuf *sb, const char *fmt, ...);
CORVUSDEF void sbcatp(StrBuf *sb, const void *ptr, int len);
CORVUSDEF void sbcatv(StrBuf *sb, const StrView sv);
CORVUSDEF void sbinss(StrBuf *sb, int pos, const char *cstr);
CORVUSDEF void sbinsc(StrBuf *sb, char c);
CORVUSDEF void sbinsf(StrBuf *sb, int pos, const char *fmt, ...);
CORVUSDEF void sbinsp(StrBuf *sb, int pos, const void *ptr, int len);
CORVUSDEF void sbinsv(StrBuf *sb, int pos, const StrView sv);
CORVUSDEF void sbdel(StrBuf *sb, int pos, int len);
CORVUSDEF void sbset(StrBuf *sb, int pos, char c);
CORVUSDEF void sbrev(StrBuf *sb);
CORVUSDEF void sbrep(StrBuf *sb, const char *cstr, int times);
CORVUSDEF void sbtoupper(StrBuf *sb);
CORVUSDEF void sbtolower(StrBuf *sb);
CORVUSDEF const char *sbascstr(StrBuf *sb);
CORVUSDEF StrView sbview(const StrBuf *sb);
#define sbforeach(sb) for (const char *it = (sb).chars; it < (sb).chars + (sb).len; it++)

/* ring buffer */

typedef struct {
    int wptr;
    int rptr;
    int len;
    int cap;
} RingBufferHeader;

#define RingBuffer(T) T *
#define rbufhdr(r) ((RingBufferHeader*)((char*)r-sizeof(RingBufferHeader)))
#define rbuflen(r) ((r) ? rbufhdr(r)->len : 0)
#define rbufcap(r) ((r) ? rbufhdr(r)->cap : 0)
#define rbufinit(r, sz) do { \
    if (!r) { \
        RingBufferHeader *h = CORVUS_REALLOC(NULL, (sz)*sizeof(*(r))); \
        CORVUS_ASSERT(h && "run out of memory"); \
        h->wptr = 0; \
        h->rptr = 0; \
        h->len = 0; \
        h->cap = (sz); \
        (r) = (void *) (h + 1); \
    } \
} while (0)
#define rbuffree(r) do { CORVUS_ASSERT(r); CORVUS_FREE(rbufhdr(r)); } while (0)
#define rbufclear(r) do { \
    CORVUS_ASSERT(r); \
    RingBufferHeader *h = rbufhdr(r); \
    h->wptr = h->rptr = h->len = 0; \
} while (0)
#define rbufput(r, e) do { \
    CORVUS_ASSERT(r); \
    RingBufferHeader *h = rbufhdr(r); \
    (r)[wrapinc_post(&h->wptr, h->cap)] = (e); \
    h->len++; \
} while (0)
#define rbufget(r) (CORVUS_ASSERT(rbuflen(r) > 0), rbufhdr(r)->len--, (r)[wrapinc_post(&rbufhdr(r)->rptr, rbufcap(r))])
#define rbufbegin(r) (CORVUS_ASSERT(r), (r)+rbufhdr(r)->rptr)
#define rbufend(r) (CORVUS_ASSERT(r), (r)+rbufhdr(r)->wptr)
#define rbufforeach(T, r) for (T *it = rbufbegin(r); it != rbufend(r); it = (r) + wrapinc(it-r, rbufcap(r)))

/* deque */

typedef struct {
    int front;
    int rear;
    int len;
    int cap;
} DequeHeader;

#define Deque(T) T *
#define deqhdr(q) ((DequeHeader*)((char*)q-sizeof(DequeHeader)))
#define deqlen(q) ((q) ? deqhdr(q)->len : 0)
#define deqcap(q) ((q) ? deqhdr(q)->cap : 0)
/* Note: after calling realloc, the old q becomes invalid.
   Always capture the old state (capacity, etc.) before
   reallocation, and never use the old q afterwards. */
#define deqkeep(q, sz) do { \
    if (deqcap(q) < (sz)) { \
        int newcap = (sz); \
        int elemsz = sizeof(*(q)); \
        bool init = (q) == NULL; \
        DequeHeader *h = init ? NULL: deqhdr(q); \
        h = CORVUS_REALLOC(h, sizeof(DequeHeader)+newcap*elemsz); \
        CORVUS_ASSERT(h && "run out of memory"); \
        if (init) { \
            h->front = 0; \
            h->rear = 0; \
            h->len = 0; \
        } else if (h->rear <= h->front) { \
            char *data = (char *) (h + 1); \
            memmove(data + h->cap*elemsz, data, h->rear*elemsz); \
            h->rear += h->cap; \
        } \
        h->cap = newcap; \
        (q) = (void *) (h + 1); \
    } \
} while (0)
#define deqfront(q) (CORVUS_ASSERT(q), (q)[deqhdr(q)->front])
#define deqrear(q) (CORVUS_ASSERT(q), (q)[wrapdec(deqhdr(q)->rear, deqcap(q))])
#define deqpushb(q, e) do { \
    if (deqcap(q) == 0) { \
        deqkeep(q, CORVUS_INITCAP); \
    } else if (deqlen(q) + 1 > deqcap(q)) { \
        deqkeep(q, 2 * deqcap(q)); \
    } \
    DequeHeader *h = deqhdr(q); \
    (q)[wrapinc_post(&h->rear, h->cap)] = (e); \
    h->len++; \
} while (0)
#define deqpushf(q, e) do { \
    if (deqcap(q) == 0) { \
        deqkeep(q, CORVUS_INITCAP); \
    } else if (deqlen(q) + 1 > deqcap(q)) { \
        deqkeep(q, 2 * deqcap(q)); \
    } \
    DequeHeader *h = deqhdr(q); \
    (q)[wrapdec_pre(&h->front, h->cap)] = (e); \
    h->len++; \
} while (0)
#define deqpopb(q) (CORVUS_ASSERT(deqlen(q) > 0), deqhdr(q)->len--, (q)[wrapdec_pre(&deqhdr(q)->rear, deqcap(q))])
#define deqpopf(q) (CORVUS_ASSERT(deqlen(q) > 0), deqhdr(q)->len--, (q)[wrapinc_post(&deqhdr(q)->front, deqcap(q))])
#define deqfree(q) do { CORVUS_ASSERT(q); CORVUS_FREE(deqhdr(q)); } while (0)
#define deqclear(q) do { \
    CORVUS_ASSERT(q); \
    DequeHeader *h = deqhdr(q); \
    h->front = h->rear = h->len = 0; \
} while (0)
#define deqbegin(q) (CORVUS_ASSERT(q), (q)+deqhdr(q)->front)
#define deqend(q) (CORVUS_ASSERT(q), (q)+deqhdr(q)->rear)
#define deqforeach(T, q) for (T *it = deqbegin(q); it != deqend(q); it = (q) + wrapinc(it-q, deqcap(q)))

/* math */

#define max(a, b)       ((a) > (b) ? (a) : (b))
#define min(a, b)       ((a) > (b) ? (b) : (a))
#define clamp(x, l, h)  (min(max((x), (l)), (h)))
#define abs(x)          ((x) < 0 ? -(x) : (x))
#define sign(x)         (((x) > 0) - ((x) < 0)) /* -1, 0, 1 */
#define swap(T, a, b)   do { T t = (a); (a) = (b); (b) = t; } while (0)
#define wrapinc(x, n)   ((x) + 1 == (n) ? 0 : (x) + 1)
#define wrapdec(x, n)   ((x) == 0 ? (n) - 1 : (x) - 1)
static inline int wrapinc_pre(int *x, int n)  { return *x = wrapinc(*x, n); }
static inline int wrapdec_pre(int *x, int n)  { return *x = wrapdec(*x, n); }
static inline int wrapinc_post(int *x, int n) { int old = *x; *x = wrapinc(*x, n); return old; }
static inline int wrapdec_post(int *x, int n) { int old = *x; *x = wrapdec(*x, n); return old; }

/* memory */

#define offset_of(type, member)         ((size_t)&(((type*)0)->member))
#define container_of(ptr, type, member) ((type*)((char*)(ptr)-offset_of(type, member)))
#define align_up(x, align)              (((x) + (align) - 1) & ~((align) - 1))
#define align_down(x, align)            ((x) & ~((align) - 1))

/* log */

typedef enum {
    CORVUS_DEBUG,
    CORVUS_INFO,
    CORVUS_WARN,
    CORVUS_ERROR,
} LogLevel;

#ifndef CORVUS_NO_LOG
    #define log(level, fmt, ...) do { \
        const char *prefix = "[UNKNOWN]"; \
        switch (level) { \
        case CORVUS_DEBUG: prefix = "[DEBUG]"; break; \
        case CORVUS_INFO:  prefix = "[INFO]";  break; \
        case CORVUS_WARN:  prefix = "[WARN]";  break; \
        case CORVUS_ERROR: prefix = "[ERROR]"; break; \
        } \
        fprintf(stderr, "%s " fmt "\n", prefix, ##__VA_ARGS__); \
    } while (0)
    #define TODO(fmt, ...)        fprintf(stderr, "TODO: " .. fmt "at %s:%d\n", ##__VA_ARGS, __FILE__, __LINE__)
    #define UNREACHABLE(fmt, ...) fprintf(stderr, "UNREACHABLE: " .. fmt "at %s:%d\n", ##__VA_ARGS, __FILE__, __LINE__)
#else
    #define log(leve, fmt, ...)
    #define TODO(fmt, ...)
    #define UNREACHABLE(fmt, ...)
#endif

#endif /* CORVUS_H */

#ifdef CORVUS_IMPLEMENTATION
#endif /* CORVUS_IMPLEMENTATION */
