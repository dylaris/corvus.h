#define KUT_IMPLEMENTATION
#define KUT_INIT_CAPACITY 4
#include "kut.h"

int main(void)
{
    int *deq = KUT_DEQUE_INITIALIZER;

    for (int i = 0; i < 32; i++) deqpushback(deq, i);
    for (int i = 80; i > 0; i--) deqpushfront(deq, i);
    printf("front: %d\n", deqfront(deq));
    printf("rear:  %d\n", deqrear(deq));
    printf("len:   %d\n", deqlen(deq));
    printf("cap:   %d\n", deqcap(deq));

    deqforeach(int, deq) { printf("%d ", *it); } printf("\n");
    printf("%d\n", deqhdr(deq)->front);
    printf("%d\n", deqhdr(deq)->rear);

    for (int i = 0; i < 20; i++) { printf("%d ", deqpopfront(deq)); } printf("\n");
    for (int i = 0; i < 20; i++) { printf("%d ", deqpopback(deq)); } printf("\n");

    printf("front: %d\n", deqfront(deq));
    printf("rear:  %d\n", deqrear(deq));
    printf("len:   %d\n", deqlen(deq));
    printf("cap:   %d\n", deqcap(deq));

    deqforeach(int, deq) { printf("%d ", *it); } printf("\n");

    deqclear(deq);
    printf("%d\n", deqhdr(deq)->front);
    printf("%d\n", deqhdr(deq)->rear);
    printf("len: %d\n", deqlen(deq));
    printf("cap: %d\n", deqcap(deq));

    deqfree(deq);
    return 0;
}
