#define CORVUS_IMPLEMENTATION
#include "corvus.h"

int main(void)
{
    const char *str = "  nice ;to ;see ;you  \r\n";

    StrView sv = svfromcstr(str);
    printf("'%.*s'\n", sv.len, sv.data);

    sv = svchopn(sv, "\n\r");
    printf("'%.*s'\n", sv.len, sv.data);

    sv = svtrim(sv);
    printf("'%.*s'\n", sv.len, sv.data);

    // Array(StrView) words = svtoks(sv, " ;");
    // Array(StrView) words = svtokv(sv, svfromcstr(" ;"));
    Array(StrView) words = svtokc(sv, ';');

    arrforeach(StrView, words) {
        printf("[%d] '" svfmt "'\n", arroff(words, it), svarg(*it));
    }

    return 0;
}
