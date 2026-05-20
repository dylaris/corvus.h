#define CORVUS_IMPLEMENTATION
#include "corvus.h"

int main(void)
{
    RingBuffer(int) rbuf = NULL;
    rbufinit(rbuf, 32);

    printf("len: %d\n", rbuflen(rbuf));
    printf("cap: %d\n", rbufcap(rbuf));

    for (int i = 0; i < 10; i++) rbufput(rbuf, i);
    printf("len: %d\n", rbuflen(rbuf));
    printf("cap: %d\n", rbufcap(rbuf));

    for (int i = 0; i < 5; i++) { printf("%d ", rbufget(rbuf)); } printf("\n");
    printf("len: %d\n", rbuflen(rbuf));
    printf("cap: %d\n", rbufcap(rbuf));

    rbufclear(rbuf);
    for (int i = 0; i < 16; i++) rbufput(rbuf, i);
    printf("len: %d\n", rbuflen(rbuf));
    printf("cap: %d\n", rbufcap(rbuf));

    int len = rbuflen(rbuf);
    for (int i = 0; i < len; i++) { printf("%d ", rbufget(rbuf)); } printf("\n");
    printf("len: %d\n", rbuflen(rbuf));
    printf("cap: %d\n", rbufcap(rbuf));

    rbuffree(rbuf);
    return 0;
}
