#define CORVUS_IMPLEMENTATION
#define CORVUS_INITCAP 4
#include "corvus.h"

int main(void)
{
    Deque(int) deq = NULL;

    for (int i = 0; i < 32; i++) deqpushb(deq, i);
    for (int i = 80; i > 0; i--) deqpushf(deq, i);
    printf("front: %d\n", deqfront(deq));
    printf("rear:  %d\n", deqrear(deq));
    printf("len:   %d\n", deqlen(deq));
    printf("cap:   %d\n", deqcap(deq));

    deqforeach(int, deq) { printf("%d ", *it); } printf("\n");
    printf("%d\n", deqhdr(deq)->front);
    printf("%d\n", deqhdr(deq)->rear);

    for (int i = 0; i < 20; i++) { printf("%d ", deqpopf(deq)); } printf("\n");
    for (int i = 0; i < 20; i++) { printf("%d ", deqpopb(deq)); } printf("\n");

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
