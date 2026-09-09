#ifndef KUT_H
#define KUT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include <stdarg.h>

#ifndef KUT_API
#define KUT_API
#endif /* KUT_API */

#ifndef KUT_INIT_CAPACITY
#define KUT_INIT_CAPACITY 128
#endif /* KUT_INIT_CAPACITY */

#ifndef KUT_REALLOC
#define KUT_REALLOC realloc
#endif /* KUT_REALLOC */

#ifndef KUT_FREE
#define KUT_FREE free
#endif /* KUT_FREE */

#ifndef KUT_ASSERT
#define KUT_ASSERT assert
#endif /* KUT_ASSERT */

/* dynamic array */

struct kut_array_header {
    int len;
    int cap;
};

#define KUT_ARRAY_INITIALIZER NULL
#define kut_arrhdr(a) ((struct kut_array_header*)((char*)(a) - sizeof(struct kut_array_header)))
#define kut_arrlen(a) ((a) ? kut_arrhdr(a)->len : 0)
#define kut_arrcap(a) ((a) ? kut_arrhdr(a)->cap : 0)
#define kut_arrkeep(a, cap) do { (a) = kut_arrkeep_impl((a), sizeof(*(a)), (cap)); } while (0)
#define kut_arrpush(a, e) do { \
    int cap = kut_arrcap(a); \
    if (cap == 0) cap = KUT_INIT_CAPACITY; \
    else if (kut_arrlen(a) + 1 > cap) cap *= 2; \
    kut_arrkeep((a), cap); \
    (a)[kut_arrhdr(a)->len++] = (e); \
} while (0)
#define kut_arrpop(a) (KUT_ASSERT(kut_arrlen(a) > 0), (a)[--kut_arrhdr(a)->len])
#define kut_arrshift(a, e) do { \
    int len = kut_arrlen(a); \
    int cap = kut_arrcap(a); \
    int elemsz = sizeof(*(a)); \
    if (len == 0) { \
        kut_arrpush((a), e); \
    } else { \
        if (len + 1 > cap) kut_arrkeep((a), 2 * cap); \
        memmove((a) + 1, (a), len * elemsz); \
        (a)[0] = (e); \
        kut_arrhdr(a)->len++; \
    } \
} while (0)
#define kut_arrunshift(a) do { \
    KUT_ASSERT(kut_arrlen(a) > 0); \
    memmove((a), (a) + 1, (--kut_arrhdr(a)->len) * sizeof(*(a))); \
} while (0)
#define kut_arrback(a) (KUT_ASSERT(kut_arrlen(a) > 0), (a)[kut_arrlen(a) - 1])
#define kut_arrfront(a) (KUT_ASSERT(kut_arrlen(a) > 0), (a)[0])
#define kut_arrclear(a) do { KUT_ASSERT(a); kut_arrhdr(a)->len = 0; } while (0)
#define kut_arrfree(a) do { KUT_ASSERT(a); KUT_FREE(kut_arrhdr(a)); } while (0)
#define kut_arroff(a, p) ((int)((p) - (a)))
#define kut_arrbegin(a) (KUT_ASSERT(a), a)
#define kut_arrend(a) (KUT_ASSERT(a), (a) + kut_arrlen(a))
#define kut_arrforeach(T, a) for (T *it = kut_arrbegin(a); it < kut_arrend(a); it++)
KUT_API void *kut_arrkeep_impl(void *a /* nullable */, int elemsz, int cap);

/* string view & string buffer */

struct kut_string_view {
    const char *data;
    int len;
};

struct kut_stkut_ring_buffer {
    char *data;
    int len;
    int cap;
};

struct kut_string_find_option {
    union {
        char ch;
        const char *cstr;
        struct {
            const void *ptr;
            int len;
        };
        struct kut_string_view sv;
    };
    bool cont;
};

#define kut_svfmt "%.*s"
#define kut_svarg(sv) (sv).len, (sv).data
KUT_API struct kut_string_view kut_svfromcstr(const char *cstr);
KUT_API struct kut_string_view kut_svfromsbuf(const struct kut_stkut_ring_buffer *sb);
KUT_API bool kut_sveqs(struct kut_string_view sv, const char *cstr);
KUT_API bool kut_sveqc(struct kut_string_view sv, char ch);
KUT_API bool kut_sveqp(struct kut_string_view sv, const void *ptr, int len);
KUT_API bool kut_sveqv(struct kut_string_view a, struct kut_string_view b);
KUT_API struct kut_string_view kut_svsub(struct kut_string_view sv, int start, int len);
#define kut_svfinds(sv, ...) kut_svfinds_impl((sv), (struct kut_string_find_option){__VA_ARGS__})
#define kut_svfindc(sv, ...) kut_svfindc_impl((sv), (struct kut_string_find_option){__VA_ARGS__})
#define kut_svfindp(sv, ...) kut_svfindp_impl((sv), (struct kut_string_find_option){__VA_ARGS__})
#define kut_svfindv(sv, ...) kut_svfindv_impl((sv), (struct kut_string_find_option){__VA_ARGS__})
KUT_API int kut_svfinds_impl(struct kut_string_view sv, struct kut_string_find_option opt);
KUT_API int kut_svfindc_impl(struct kut_string_view sv, struct kut_string_find_option opt);
KUT_API int kut_svfindp_impl(struct kut_string_view sv, struct kut_string_find_option opt);
KUT_API int kut_svfindv_impl(struct kut_string_view sv, struct kut_string_find_option opt);
KUT_API struct kut_string_view *kut_svtoks(struct kut_string_view sv, const char *cstr);
KUT_API struct kut_string_view *kut_svtokc(struct kut_string_view sv, char ch);
KUT_API struct kut_string_view *kut_svtokp(struct kut_string_view sv, const void *ptr, int len);
KUT_API struct kut_string_view *kut_svtokv(struct kut_string_view sv, struct kut_string_view sep);
KUT_API struct kut_string_view kut_svtrim(struct kut_string_view sv);
KUT_API struct kut_string_view kut_svtriml(struct kut_string_view sv);
KUT_API struct kut_string_view kut_svtrimr(struct kut_string_view sv);
KUT_API struct kut_string_view kut_svchop(struct kut_string_view sv, char c);
KUT_API struct kut_string_view kut_svchopn(struct kut_string_view sv, const char *cs);
KUT_API bool kut_svprefix(struct kut_string_view sv, struct kut_string_view prefix);
KUT_API bool kut_svsuffix(struct kut_string_view sv, struct kut_string_view suffix);
KUT_API char *kut_svtocstr(const struct kut_string_view sv);
#define kut_svbegin(sv) (KUT_ASSERT((sv).data), (sv).data)
#define kut_svend(sv) (KUT_ASSERT((sv).data), (sv).data + (sv).len)
#define kut_svforeach(sv) for (const char *it = kut_svbegin(sv); it < kut_svend(sv); it++)

KUT_API void kut_sbclear(struct kut_stkut_ring_buffer *sb);
KUT_API void kut_sbfree(struct kut_stkut_ring_buffer *sb);
KUT_API void kut_sbkeep(struct kut_stkut_ring_buffer *sb, int sz);
KUT_API struct kut_stkut_ring_buffer kut_sbfmt(const char *fmt, ...);
KUT_API void kut_sbcatsn(struct kut_stkut_ring_buffer *sb, ... /* NULL */);
KUT_API void kut_sbcatcn(struct kut_stkut_ring_buffer *sb, ... /* -1 */);
KUT_API void kut_sbcats(struct kut_stkut_ring_buffer *sb, const char *cstr);
KUT_API void kut_sbcatc(struct kut_stkut_ring_buffer *sb, char c);
KUT_API void kut_sbcatp(struct kut_stkut_ring_buffer *sb, const void *ptr, int len);
KUT_API void kut_sbcatv(struct kut_stkut_ring_buffer *sb, struct kut_string_view sv);
KUT_API void kut_sbinss(struct kut_stkut_ring_buffer *sb, int pos, const char *cstr);
KUT_API void kut_sbinsc(struct kut_stkut_ring_buffer *sb, int pos, char c);
KUT_API void kut_sbinsp(struct kut_stkut_ring_buffer *sb, int pos, const void *ptr, int len);
KUT_API void kut_sbinsv(struct kut_stkut_ring_buffer *sb, int pos, struct kut_string_view sv);
KUT_API void kut_sbdel(struct kut_stkut_ring_buffer *sb, int pos, int len);
KUT_API void kut_sbset(struct kut_stkut_ring_buffer *sb, int pos, char c);
KUT_API void kut_sbreverse(struct kut_stkut_ring_buffer *sb);
KUT_API void kut_sbrepeat(struct kut_stkut_ring_buffer *sb, const char *cstr, int times);
KUT_API void kut_sbjoin(struct kut_stkut_ring_buffer *sb, const char *sep, ... /* NULL */);
KUT_API void kut_sbtoupper(struct kut_stkut_ring_buffer *sb);
KUT_API void kut_sbtolower(struct kut_stkut_ring_buffer *sb);
KUT_API const char *kut_sbtocstr(struct kut_stkut_ring_buffer *sb);
KUT_API struct kut_string_view kut_sbtoview(const struct kut_stkut_ring_buffer *sb);
#define kut_sbbegin(sb) (KUT_ASSERT((sb).data), (sb).data)
#define kut_sbend(sb) (KUT_ASSERT((sb).data), (sb).data + (sb).len)
#define kut_sbforeach(sb) for (char *it = kut_sbbegin(sb); it < kut_sbend(sb); it++)

/* ring buffer */

struct kut_ring_buffer_header {
    int wptr;
    int rptr;
    int len;
    int cap;
};

#define KUT_RING_BUFFER_INITIALIZER NULL
#define kut_rbufhdr(r) ((struct kut_ring_buffer_header*)((char*)(r) - sizeof(struct kut_ring_buffer_header)))
#define kut_rbuflen(r) ((r) ? kut_rbufhdr(r)->len : 0)
#define kut_rbufcap(r) ((r) ? kut_rbufhdr(r)->cap : 0)
#define kut_rbufinit(r, sz) do { \
    if (!(r)) { \
        struct kut_ring_buffer_header *h; \
        h = KUT_REALLOC(NULL, (sz) * sizeof(*(r))); \
        KUT_ASSERT(h && "run out of memory"); \
        h->wptr = 0; \
        h->rptr = 0; \
        h->len = 0; \
        h->cap = (sz); \
        (r) = (void *)(h + 1); \
    } \
} while (0)
#define kut_rbuffree(r) do { KUT_ASSERT(r); KUT_FREE(kut_rbufhdr(r)); } while (0)
#define kut_rbufclear(r) do { \
    struct kut_ring_buffer_header *h; \
    KUT_ASSERT(r); \
    h = kut_rbufhdr(r); \
    h->wptr = h->rptr = h->len = 0; \
} while (0)
#define kut_rbufput(r, e) do { \
    struct kut_ring_buffer_header *h; \
    KUT_ASSERT(r); \
    h = kut_rbufhdr(r); \
    (r)[kut_wrapinc_post(&h->wptr, h->cap)] = (e); \
    h->len++; \
} while (0)
#define kut_rbufget(r) (KUT_ASSERT(kut_rbuflen(r) > 0), kut_rbufhdr(r)->len--, (r)[kut_wrapinc_post(&kut_rbufhdr(r)->rptr, kut_rbufcap(r))])
#define kut_rbufbegin(r) (KUT_ASSERT(r), (r) + kut_rbufhdr(r)->rptr)
#define kut_rbufend(r) (KUT_ASSERT(r), (r) + kut_rbufhdr(r)->wptr)
#define kut_rbufforeach(T, r) for (T *it = kut_rbufbegin(r); it != kut_rbufend(r); it = (r) + kut_wrapinc(it - (r), kut_rbufcap(r)))

/* deque */

struct kut_deque_header {
    int front;
    int rear;
    int len;
    int cap;
};

#define KUT_DEQUE_INITIALIZER NULL
#define kut_deqhdr(q) ((struct kut_deque_header*)((char*)(q) - sizeof(struct kut_deque_header)))
#define kut_deqlen(q) ((q) ? kut_deqhdr(q)->len : 0)
#define kut_deqcap(q) ((q) ? kut_deqhdr(q)->cap : 0)
#define kut_deqkeep(q, cap) do { (q) = kut_deqkeep_impl((q), sizeof(*(q)), (cap)); } while (0)
#define kut_deqfront(q) (KUT_ASSERT(q), (q)[kut_deqhdr(q)->front])
#define kut_deqrear(q) (KUT_ASSERT(q), (q)[kut_wrapdec(kut_deqhdr(q)->rear, kut_deqcap(q))])
#define kut_deqpushback(q, e) do { \
    struct kut_deque_header *h; \
    if (kut_deqcap(q) == 0) { \
        kut_deqkeep(q, KUT_INIT_CAPACITY); \
    } else if (kut_deqlen(q) + 1 > kut_deqcap(q)) { \
        kut_deqkeep(q, 2 * kut_deqcap(q)); \
    } \
    h = kut_deqhdr(q); \
    (q)[kut_wrapinc_post(&h->rear, h->cap)] = (e); \
    h->len++; \
} while (0)
#define kut_deqpushfront(q, e) do { \
    struct kut_deque_header *h; \
    if (kut_deqcap(q) == 0) { \
        kut_deqkeep(q, KUT_INIT_CAPACITY); \
    } else if (kut_deqlen(q) + 1 > kut_deqcap(q)) { \
        kut_deqkeep(q, 2 * kut_deqcap(q)); \
    } \
    h = kut_deqhdr(q); \
    (q)[kut_wrapdec_pre(&h->front, h->cap)] = (e); \
    h->len++; \
} while (0)
#define kut_deqpopback(q) (KUT_ASSERT(kut_deqlen(q) > 0), kut_deqhdr(q)->len--, (q)[kut_wrapdec_pre(&kut_deqhdr(q)->rear, kut_deqcap(q))])
#define kut_deqpopfront(q) (KUT_ASSERT(kut_deqlen(q) > 0), kut_deqhdr(q)->len--, (q)[kut_wrapinc_post(&kut_deqhdr(q)->front, kut_deqcap(q))])
#define kut_deqfree(q) do { KUT_ASSERT(q); KUT_FREE(kut_deqhdr(q)); } while (0)
#define kut_deqclear(q) do { \
    struct kut_deque_header *h; \
    KUT_ASSERT(q); \
    h = kut_deqhdr(q); \
    h->front = h->rear = h->len = 0; \
} while (0)
#define kut_deqbegin(q) (KUT_ASSERT(q), (q) + kut_deqhdr(q)->front)
#define kut_deqend(q) (KUT_ASSERT(q), (q) + kut_deqhdr(q)->rear)
#define kut_deqforeach(T, q) for (T *it = kut_deqbegin(q); it != kut_deqend(q); it = (q) + kut_wrapinc(it - (q), kut_deqcap(q)))
KUT_API void *kut_deqkeep_impl(void *q /* nullable */, int elemsz, int cap);

/* math */

#define kut_max(a, b)       ((a) > (b) ? (a) : (b))
#define kut_min(a, b)       ((a) > (b) ? (b) : (a))
#define kut_clamp(x, l, h)  (kut_min(kut_max((x), (l)), (h)))
#define kut_abs(x)          ((x) < 0 ? -(x) : (x))
#define kut_sign(x)         (((x) > 0) - ((x) < 0)) /* -1, 0, 1 */
#define kut_swap(T, a, b)   do { T t = (a); (a) = (b); (b) = t; } while (0)
#define kut_wrapinc(x, n)   ((x) + 1 == (n) ? 0 : (x) + 1)
#define kut_wrapdec(x, n)   ((x) == 0 ? (n) - 1 : (x) - 1)
KUT_API int kut_wrapinc_post(int *x, int n);
KUT_API int kut_wrapinc_pre(int *x, int n);
KUT_API int kut_wrapdec_post(int *x, int n);
KUT_API int kut_wrapdec_pre(int *x, int n);

/* memory */

#define kut_offset_of(type, member)         ((size_t)&(((type*)0)->member))
#define kut_container_of(ptr, type, member) ((type*)((char*)(ptr) - kut_offset_of(type, member)))
#define kut_align_up(x, align)              (((x) + (align) - 1) & ~((align) - 1))
#define kut_align_down(x, align)            ((x) & ~((align) - 1))

/* log */

enum kut_log_level {
    KUT_DEBUG,
    KUT_INFO,
    KUT_WARN,
    KUT_ERROR,
};

#ifndef KUT_NO_LOG
#define kut_log(level, fmt, ...) do { \
    const char *prefix = "[UNKNOWN]"; \
    switch (level) { \
    case KUT_DEBUG: prefix = "[DEBUG]"; break; \
    case KUT_INFO:  prefix = "[INFO]";  break; \
    case KUT_WARN:  prefix = "[WARN]";  break; \
    case KUT_ERROR: prefix = "[ERROR]"; break; \
    } \
    fprintf(stderr, "%s " fmt "\n", prefix, ##__VA_ARGS__); \
} while (0)
#define kut_todo(fmt, ...)        (fprintf(stderr, "TODO: " fmt " at %s:%d\n", ##__VA_ARGS__, __FILE__, __LINE__), abort())
#define kut_unreachable(fmt, ...) (fprintf(stderr, "UNREACHABLE: " fmt " at %s:%d\n", ##__VA_ARGS__, __FILE__, __LINE__), abort())
#else
#define kut_log(level, fmt, ...)
#define kut_todo(fmt, ...)
#define kut_unreachable(fmt, ...)
#endif

#endif /* KUT_H */

#ifdef KUT_IMPLEMENTATION

KUT_API void *kut_arrkeep_impl(void *a /* nullable */, int elemsz, int cap)
{
    bool first;
    struct kut_array_header *h;

    if (kut_arrcap(a) >= cap) return a;

    first = a == NULL;
    cap = first ? KUT_INIT_CAPACITY : cap;
    h = first ? NULL : kut_arrhdr(a);
    h = KUT_REALLOC(h, sizeof(struct kut_array_header) + cap * elemsz);
    KUT_ASSERT(h && "run out of memory");

    if (first) h->len = 0;
    h->cap = cap;

    return (void *)(h + 1);
}

KUT_API void *kut_deqkeep_impl(void *q /* nullable */, int elemsz, int cap)
{
    bool first;
    struct kut_deque_header *h;

    if (kut_deqcap(q) >= cap) return q;

    first = q == NULL;
    cap = first ? KUT_INIT_CAPACITY : cap;
    h = first ? NULL : kut_deqhdr(q);
    h = KUT_REALLOC(h, sizeof(struct kut_deque_header) + cap * elemsz);
    KUT_ASSERT(h && "run out of memory");

    if (first) {
        h->front = 0;
        h->rear = 0;
        h->len = 0;
    } else if (h->rear <= h->front) {
        char *data = (char *)(h + 1);
        memmove(data + h->cap * elemsz, data, h->rear * elemsz);
        h->rear += h->cap;
    }
    h->cap = cap;
    return (void *)(h + 1);
}

KUT_API struct kut_string_view kut_svfromcstr(const char *cstr)
{
    struct kut_string_view result = {
        .data = cstr,
        .len = (int)strlen(cstr)
    };
    return result;
}

KUT_API struct kut_string_view kut_svfromsbuf(const struct kut_stkut_ring_buffer *sb)
{
    struct kut_string_view result = {
        .data = sb->data,
        .len = sb->len
    };
    return result;
}

KUT_API bool kut_sveqv(struct kut_string_view a, struct kut_string_view b)
{
    if (a.len != b.len) return false;
    return memcmp(a.data, b.data, a.len) == 0;
}

KUT_API bool kut_sveqs(struct kut_string_view sv, const char *cstr)
{
    int len = (int)strlen(cstr);
    if (sv.len != len) return false;
    return memcmp(sv.data, cstr, len) == 0;
}

KUT_API bool kut_sveqc(struct kut_string_view sv, char ch)
{
    if (sv.len != 1) return false;
    return *sv.data == ch;
}

KUT_API bool kut_sveqp(struct kut_string_view sv, const void *ptr, int len)
{
    if (sv.len != len) return false;
    return memcmp(sv.data, ptr, len) == 0;
}

KUT_API struct kut_string_view kut_svsub(struct kut_string_view sv, int start, int len)
{
    struct kut_string_view result = {
        .data = sv.data + start,
        .len = len
    };
    return result;
}

KUT_API int kut_svfindc_impl(struct kut_string_view sv, struct kut_string_find_option opt)
{
    static struct { int pos; char ch; } state;

    if (!opt.cont) {
        state.pos = 0;
        state.ch = opt.ch;
    }

    for (int i = state.pos; i < sv.len; i++) {
        if (sv.data[i] != state.ch) continue;
        state.pos = i + 1;
        return i;
    }
    return -1;
}

#define kut_sv_find_part_(ptr_, len_) \
    static struct { int pos; const void *ptr; int len; } state; \
    if (!opt.cont) { \
        state.pos = 0; \
        state.ptr = (ptr_); \
        state.len = (len_); \
    } \
    if (state.len == 0 || state.len > sv.len) return -1; \
    { \
        for (int i = state.pos; i < sv.len; i++) { \
            if (memcmp(sv.data + i, state.ptr, state.len) == 0) { \
                state.pos = i + state.len; \
                return i; \
            } \
        } \
    } \
    return -1;

KUT_API int kut_svfinds_impl(struct kut_string_view sv, struct kut_string_find_option opt)
{
    kut_sv_find_part_(opt.cstr, strlen(opt.cstr))
}

KUT_API int kut_svfindv_impl(struct kut_string_view sv, struct kut_string_find_option opt)
{
    kut_sv_find_part_(opt.sv.data, opt.sv.len)
}

KUT_API int kut_svfindp_impl(struct kut_string_view sv, struct kut_string_find_option opt)
{
    kut_sv_find_part_(opt.ptr, opt.len)
}

#undef kut_sv_find_part_

KUT_API struct kut_string_view *kut_svtoks(struct kut_string_view sv, const char *cstr)
{
    struct kut_string_view *toks;
    int len;
    int left;
    int right;
    struct kut_string_view tok;

    toks = NULL;
    len = (int)strlen(cstr);
    left = 0;
    right = kut_svfinds(sv, .cstr = cstr);
    if (right == -1) right = sv.len;

    tok = kut_svsub(sv, left, right - left);
    kut_arrpush(toks, tok);
    left = right + len;

    while (left < sv.len) {
        right = kut_svfinds(sv, .cont = true);
        if (right == -1) right = sv.len;
        tok = kut_svsub(sv, left, right - left);
        kut_arrpush(toks, tok);
        left = right + len;
    }

    return toks;
}

KUT_API struct kut_string_view *kut_svtokc(struct kut_string_view sv, char ch)
{
    struct kut_string_view *toks;
    int len;
    int left;
    int right;
    struct kut_string_view tok;

    toks = NULL;
    len = 1;
    left = 0;
    right = kut_svfindc(sv, .ch = ch);
    if (right == -1) right = sv.len;

    tok = kut_svsub(sv, left, right - left);
    kut_arrpush(toks, tok);
    left = right + len;

    while (left < sv.len) {
        right = kut_svfindc(sv, .cont = true);
        if (right == -1) right = sv.len;
        tok = kut_svsub(sv, left, right - left);
        kut_arrpush(toks, tok);
        left = right + len;
    }

    return toks;
}

KUT_API struct kut_string_view *kut_svtokp(struct kut_string_view sv, const void *ptr, int len)
{
    struct kut_string_view *toks;
    int left;
    int right;
    struct kut_string_view tok;

    toks = NULL;
    left = 0;
    right = kut_svfindp(sv, .ptr = ptr, .len = len);
    if (right == -1) right = sv.len;

    tok = kut_svsub(sv, left, right - left);
    kut_arrpush(toks, tok);
    left = right + len;

    while (left < sv.len) {
        right = kut_svfindp(sv, .cont = true);
        if (right == -1) right = sv.len;
        tok = kut_svsub(sv, left, right - left);
        kut_arrpush(toks, tok);
        left = right + len;
    }

    return toks;
}

KUT_API struct kut_string_view *kut_svtokv(struct kut_string_view sv, struct kut_string_view sep)
{
    struct kut_string_view *toks;
    int len;
    int left;
    int right;
    struct kut_string_view tok;

    toks = NULL;
    len = sep.len;
    left = 0;
    right = kut_svfindv(sv, .sv = sep);
    if (right == -1) right = sv.len;

    tok = kut_svsub(sv, left, right - left);
    kut_arrpush(toks, tok);
    left = right + len;

    while (left < sv.len) {
        right = kut_svfindv(sv, .cont = true);
        if (right == -1) right = sv.len;
        tok = kut_svsub(sv, left, right - left);
        kut_arrpush(toks, tok);
        left = right + len;
    }

    return toks;
}

static inline bool kut_iswhitespace(char c)
{
    return c == ' ' || c == '\t' || c == '\r' || c == '\n';
}

KUT_API struct kut_string_view kut_svtrim(struct kut_string_view sv)
{
    int start;
    int end;
    struct kut_string_view result;

    start = 0;
    end = sv.len;
    while (start < end && kut_iswhitespace(sv.data[start])) start++;
    while (end > start && kut_iswhitespace(sv.data[end - 1])) end--;
    result.data = sv.data + start;
    result.len = end - start;
    return result;
}

KUT_API struct kut_string_view kut_svtriml(struct kut_string_view sv)
{
    int start;
    struct kut_string_view result;

    start = 0;
    while (start < sv.len && kut_iswhitespace(sv.data[start])) start++;
    result.data = sv.data + start;
    result.len = sv.len - start;
    return result;
}

KUT_API struct kut_string_view kut_svtrimr(struct kut_string_view sv)
{
    int end;
    struct kut_string_view result;

    end = sv.len;
    while (end > 0 && kut_iswhitespace(sv.data[end - 1])) end--;
    result.data = sv.data;
    result.len = end;
    return result;
}

KUT_API struct kut_string_view kut_svchop(struct kut_string_view sv, char c)
{
    int i;
    struct kut_string_view result;

    i = sv.len;
    while (i > 0 && sv.data[i - 1] == c) i--;
    result.data = sv.data;
    result.len = i;
    return result;
}

KUT_API struct kut_string_view kut_svchopn(struct kut_string_view sv, const char *cs)
{
    int i;
    bool found;
    const char *c;
    struct kut_string_view result;

    i = sv.len;
    while (i > 0) {
        found = false;
        for (c = cs; *c != '\0'; c++) {
            if (sv.data[i - 1] == *c) {
                found = true;
                break;
            }
        }
        if (!found) break;
        i--;
    }
    result.data = sv.data;
    result.len = i;
    return result;
}

KUT_API bool kut_svprefix(struct kut_string_view sv, struct kut_string_view prefix)
{
    if (prefix.len > sv.len) return false;
    return memcmp(sv.data, prefix.data, prefix.len) == 0;
}

KUT_API bool kut_svsuffix(struct kut_string_view sv, struct kut_string_view suffix)
{
    if (suffix.len > sv.len) return false;
    return memcmp(sv.data + sv.len - suffix.len, suffix.data, suffix.len) == 0;
}

KUT_API char *kut_svtocstr(const struct kut_string_view sv)
{
    char *cstr = KUT_REALLOC(NULL, sv.len + 1);
    KUT_ASSERT(cstr && "run out of memory");
    memcpy(cstr, sv.data, sv.len);
    cstr[sv.len] = '\0';
    return cstr;
}

KUT_API void kut_sbclear(struct kut_stkut_ring_buffer *sb)
{
    sb->len = 0;
}

KUT_API void kut_sbfree(struct kut_stkut_ring_buffer *sb)
{
    if (!sb->data) return;
    KUT_FREE(sb->data);
    sb->data = NULL;
    sb->len = sb->cap = 0;
}

static void kut__sbgrow(struct kut_stkut_ring_buffer *sb, int needed)
{
    int newcap;
    char *newdata;

    if (sb->len + needed <= sb->cap) return;
    newcap = sb->cap == 0 ? KUT_INIT_CAPACITY : sb->cap;
    while (newcap < sb->len + needed) newcap *= 2;
    newdata = KUT_REALLOC(sb->data, newcap);
    KUT_ASSERT(newdata && "run out of memory");
    sb->data = newdata;
    sb->cap = newcap;
}

KUT_API void kut_sbkeep(struct kut_stkut_ring_buffer *sb, int sz)
{
    if (sz > sb->cap) kut__sbgrow(sb, sz - sb->len);
}

KUT_API void kut_sbcatsn(struct kut_stkut_ring_buffer *sb, ... /* NULL */)
{
    va_list args;
    const char *str;

    va_start(args, sb);
    str = va_arg(args, const char *);
    while (str != NULL) {
        kut_sbcats(sb, str);
        str = va_arg(args, const char *);
    }
    va_end(args);
}

KUT_API void kut_sbcatcn(struct kut_stkut_ring_buffer *sb, ... /* -1 */)
{
    va_list args;
    int c;

    va_start(args, sb);
    c = va_arg(args, int);
    while (c != -1) {
        kut_sbcatc(sb, (char)c);
        c = va_arg(args, int);
    }
    va_end(args);
}

KUT_API void kut_sbcats(struct kut_stkut_ring_buffer *sb, const char *cstr)
{
    int len = (int)strlen(cstr);
    kut__sbgrow(sb, len);
    memcpy(sb->data + sb->len, cstr, len);
    sb->len += len;
}

KUT_API void kut_sbcatc(struct kut_stkut_ring_buffer *sb, char c)
{
    kut__sbgrow(sb, 1);
    sb->data[sb->len++] = c;
}

KUT_API struct kut_stkut_ring_buffer kut_sbfmt(const char *fmt, ...)
{
    va_list args;
    int len;
    struct kut_stkut_ring_buffer sb;

    memset(&sb, 0, sizeof(sb));
    va_start(args, fmt);
    len = vsnprintf(NULL, 0, fmt, args);
    va_end(args);
    kut__sbgrow(&sb, len + 1);
    va_start(args, fmt);
    vsnprintf(sb.data + sb.len, len + 1, fmt, args);
    va_end(args);
    sb.len += len;
    return sb;
}

KUT_API void kut_sbcatp(struct kut_stkut_ring_buffer *sb, const void *ptr, int len)
{
    kut__sbgrow(sb, len);
    memcpy(sb->data + sb->len, ptr, len);
    sb->len += len;
}

KUT_API void kut_sbcatv(struct kut_stkut_ring_buffer *sb, struct kut_string_view sv)
{
    kut_sbcatp(sb, sv.data, sv.len);
}

KUT_API void kut_sbinss(struct kut_stkut_ring_buffer *sb, int pos, const char *cstr)
{
    int len = (int)strlen(cstr);
    kut__sbgrow(sb, len);
    memmove(sb->data + pos + len, sb->data + pos, sb->len - pos);
    memcpy(sb->data + pos, cstr, len);
    sb->len += len;
}

KUT_API void kut_sbinsc(struct kut_stkut_ring_buffer *sb, int pos, char c)
{
    kut__sbgrow(sb, 1);
    memmove(sb->data + pos + 1, sb->data + pos, sb->len - pos);
    sb->data[pos] = c;
    sb->len += 1;
}

KUT_API void kut_sbinsp(struct kut_stkut_ring_buffer *sb, int pos, const void *ptr, int len)
{
    kut__sbgrow(sb, len);
    memmove(sb->data + pos + len, sb->data + pos, sb->len - pos);
    memcpy(sb->data + pos, ptr, len);
    sb->len += len;
}

KUT_API void kut_sbinsv(struct kut_stkut_ring_buffer *sb, int pos, struct kut_string_view sv)
{
    kut_sbinsp(sb, pos, sv.data, sv.len);
}

KUT_API void kut_sbdel(struct kut_stkut_ring_buffer *sb, int pos, int len)
{
    if (len < 0) len = (sb->len + len) - pos;
    memmove(sb->data + pos, sb->data + pos + len, sb->len - pos - len);
    sb->len -= len;
}

KUT_API void kut_sbset(struct kut_stkut_ring_buffer *sb, int pos, char c)
{
    sb->data[pos] = c;
}

KUT_API void kut_sbreverse(struct kut_stkut_ring_buffer *sb)
{
    for (int i = 0, j = sb->len - 1; i < j; i++, j--) {
        char t = sb->data[i];
        sb->data[i] = sb->data[j];
        sb->data[j] = t;
    }
}

KUT_API void kut_sbrepeat(struct kut_stkut_ring_buffer *sb, const char *cstr, int times)
{
    int len = (int)strlen(cstr);
    kut__sbgrow(sb, len * times);
    for (int i = 0; i < times; i++) {
        memcpy(sb->data + sb->len, cstr, len);
        sb->len += len;
    }
}

KUT_API void kut_sbjoin(struct kut_stkut_ring_buffer *sb, const char *sep, ... /* NULL */)
{
    va_list args;
    const char *str;

    va_start(args, sep);
    str = va_arg(args, const char *);
    if (str == NULL) {
        va_end(args);
        return;
    }

    kut_sbcats(sb, str);
    while ((str = va_arg(args, const char *)) != NULL) {
        kut_sbcats(sb, sep);
        kut_sbcats(sb, str);
    }
    va_end(args);
}

KUT_API void kut_sbtoupper(struct kut_stkut_ring_buffer *sb)
{
    for (int i = 0; i < sb->len; i++) {
        if (sb->data[i] >= 'a' && sb->data[i] <= 'z') {
            sb->data[i] -= 'a' - 'A';
        }
    }
}

KUT_API void kut_sbtolower(struct kut_stkut_ring_buffer *sb)
{
    for (int i = 0; i < sb->len; i++) {
        if (sb->data[i] >= 'A' && sb->data[i] <= 'Z') {
            sb->data[i] += 'a' - 'A';
        }
    }
}

KUT_API const char *kut_sbtocstr(struct kut_stkut_ring_buffer *sb)
{
    kut__sbgrow(sb, 1);
    sb->data[sb->len] = '\0';
    return sb->data;
}

KUT_API struct kut_string_view kut_sbtoview(const struct kut_stkut_ring_buffer *sb)
{
    struct kut_string_view result = {
        .data = sb->data,
        .len = sb->len
    };
    return result;
}

KUT_API int kut_wrapinc_post(int *x, int n)
{
    int oldx = *x;
    *x = kut_wrapinc(oldx, n);
    return oldx;
}

KUT_API int kut_wrapinc_pre(int *x, int n)
{
    int oldx = *x;
    *x = kut_wrapinc(oldx, n);
    return *x;
}

KUT_API int kut_wrapdec_post(int *x, int n)
{
    int oldx = *x;
    *x = kut_wrapdec(oldx, n);
    return oldx;
}

KUT_API int kut_wrapdec_pre(int *x, int n)
{
    int oldx = *x;
    *x = kut_wrapdec(oldx, n);
    return *x;
}

#endif /* KUT_IMPLEMENTATION */

#ifndef KUT_NO_STRIP_PREFIX
    #define array_header kut_array_header
    #define arrhdr kut_arrhdr
    #define arrlen kut_arrlen
    #define arrcap kut_arrcap
    #define arrkeep kut_arrkeep
    #define arrpush kut_arrpush
    #define arrpop kut_arrpop
    #define arrshift kut_arrshift
    #define arrunshift kut_arrunshift
    #define arrback kut_arrback
    #define arrfront kut_arrfront
    #define arrclear kut_arrclear
    #define arrfree kut_arrfree
    #define arroff kut_arroff
    #define arrbegin kut_arrbegin
    #define arrend kut_arrend
    #define arrforeach kut_arrforeach
    #define arrkeep_impl kut_arrkeep_impl
    #define svfmt kut_svfmt
    #define svarg kut_svarg
    #define string_view kut_string_view
    #define stkut_ring_buffer kut_stkut_ring_buffer
    #define string_find_option kut_string_find_option
    #define svfromcstr kut_svfromcstr
    #define svfromsbuf kut_svfromsbuf
    #define sveqs kut_sveqs
    #define sveqc kut_sveqc
    #define sveqp kut_sveqp
    #define sveqv kut_sveqv
    #define svsub kut_svsub
    #define svfinds kut_svfinds
    #define svfindc kut_svfindc
    #define svfindp kut_svfindp
    #define svfindv kut_svfindv
    #define svfinds_impl kut_svfinds_impl
    #define svfindc_impl kut_svfindc_impl
    #define svfindp_impl kut_svfindp_impl
    #define svfindv_impl kut_svfindv_impl
    #define svtoks kut_svtoks
    #define svtokc kut_svtokc
    #define svtokp kut_svtokp
    #define svtokv kut_svtokv
    #define svtrim kut_svtrim
    #define svtriml kut_svtriml
    #define svtrimr kut_svtrimr
    #define svchop kut_svchop
    #define svchopn kut_svchopn
    #define svprefix kut_svprefix
    #define svsuffix kut_svsuffix
    #define svtocstr kut_svtocstr
    #define svbegin kut_svbegin
    #define svend kut_svend
    #define svforeach kut_svforeach
    #define sbclear kut_sbclear
    #define sbfree kut_sbfree
    #define sbkeep kut_sbkeep
    #define sbfmt kut_sbfmt
    #define sbcatsn kut_sbcatsn
    #define sbcatcn kut_sbcatcn
    #define sbcats kut_sbcats
    #define sbcatc kut_sbcatc
    #define sbcatp kut_sbcatp
    #define sbcatv kut_sbcatv
    #define sbinss kut_sbinss
    #define sbinsc kut_sbinsc
    #define sbinsp kut_sbinsp
    #define sbinsv kut_sbinsv
    #define sbdel kut_sbdel
    #define sbset kut_sbset
    #define sbreverse kut_sbreverse
    #define sbrepeat kut_sbrepeat
    #define sbjoin kut_sbjoin
    #define sbtoupper kut_sbtoupper
    #define sbtolower kut_sbtolower
    #define sbtocstr kut_sbtocstr
    #define sbtoview kut_sbtoview
    #define sbbegin kut_sbbegin
    #define sbend kut_sbend
    #define sbforeach kut_sbforeach
    #define ring_buffer_header kut_ring_buffer_header
    #define rbufhdr kut_rbufhdr
    #define rbuflen kut_rbuflen
    #define rbufcap kut_rbufcap
    #define rbufinit kut_rbufinit
    #define rbuffree kut_rbuffree
    #define rbufclear kut_rbufclear
    #define rbufput kut_rbufput
    #define rbufget kut_rbufget
    #define rbufbegin kut_rbufbegin
    #define rbufend kut_rbufend
    #define rbufforeach kut_rbufforeach
    #define deque_header kut_deque_header
    #define deque kut_deque
    #define deqhdr kut_deqhdr
    #define deqlen kut_deqlen
    #define deqcap kut_deqcap
    #define deqkeep kut_deqkeep
    #define deqfront kut_deqfront
    #define deqrear kut_deqrear
    #define deqpushback kut_deqpushback
    #define deqpushfront kut_deqpushfront
    #define deqpopback kut_deqpopback
    #define deqpopfront kut_deqpopfront
    #define deqfree kut_deqfree
    #define deqclear kut_deqclear
    #define deqbegin kut_deqbegin
    #define deqend kut_deqend
    #define deqforeach kut_deqforeach
    #define deqkeep_impl kut_deqkeep_impl
    #define max kut_max
    #define min kut_min
    #define clamp kut_clamp
    #define abs kut_abs
    #define sign kut_sign
    #define swap kut_swap
    #define wrapinc kut_wrapinc
    #define wrapdec kut_wrapdec
    #define wrapinc_post kut_wrapinc_post
    #define wrapinc_pre kut_wrapinc_pre
    #define wrapdec_post kut_wrapdec_post
    #define wrapdec_pre kut_wrapdec_pre
    #define offset_of kut_offset_of
    #define container_of kut_container_of
    #define align_up kut_align_up
    #define align_down kut_align_down
    #define log_level kut_log_level
    #define log kut_log
    #define todo kut_todo
    #define unreachable kut_unreachable
#endif /* KUT_NO_STRIP_PREFIX */
