#define CORVUS_IMPLEMENTATION
#define CORVUS_INITCAP 16
#include "corvus.h"

int main(void)
{
    Array(int) xs = NULL;

    for (int i = 0; i < 200; i++) arrpush(xs, i);
    arrunshift(xs);
    arrshift(xs, 1001);
    arrforeach(int, xs) { printf("%d ", *it); } printf("\n");
    printf("top: %d\n", arrtop(xs));
    printf("bot: %d\n", arrbot(xs));
    printf("len: %d\n", arrlen(xs));
    printf("cap: %d\n", arrcap(xs));

    arrrev(int, xs);
    arrpush(xs, 999);
    arrforeach(int, xs) { printf("%d ", *it); } printf("\n");
    printf("top: %d\n", arrtop(xs));
    printf("bot: %d\n", arrbot(xs));
    printf("len: %d\n", arrlen(xs));
    printf("cap: %d\n", arrcap(xs));

    arrclear(xs);
    printf("len: %d\n", arrlen(xs));
    printf("cap: %d\n", arrcap(xs));

    arrfree(xs);
    return 0;
}
