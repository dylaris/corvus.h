#define KUT_IMPLEMENTATION
#define KUT_INIT_CAPACITY 16
#include "kut.h"

int main(void)
{
    int *xs = KUT_ARRAY_INITIALIZER;

    for (int i = 0; i < 200; i++) arrpush(xs, i);
    arrunshift(xs);
    arrshift(xs, 1001);
    arrforeach(int, xs) { printf("%d ", *it); } printf("\n");
    printf("front: %d\n", arrfront(xs));
    printf("back: %d\n", arrback(xs));
    printf("len: %d\n", arrlen(xs));
    printf("cap: %d\n", arrcap(xs));

    arrpush(xs, 999);
    arrforeach(int, xs) { printf("%d ", *it); } printf("\n");
    printf("front: %d\n", arrfront(xs));
    printf("back: %d\n", arrback(xs));
    printf("len: %d\n", arrlen(xs));
    printf("cap: %d\n", arrcap(xs));

    arrclear(xs);
    printf("len: %d\n", arrlen(xs));
    printf("cap: %d\n", arrcap(xs));

    arrfree(xs);
    return 0;
}
