#define KUT_IMPLEMENTATION
#include "kut.h"

int main(void)
{
    const char *str = "  nice ;to ;see ;you  \r\n";

    struct string_view sv = svfromcstr(str);
    printf("'" svfmt "'\n", svarg(sv));

    sv = svchopn(sv, "\n\r");
    printf("'" svfmt "'\n", svarg(sv));

    sv = svtrim(sv);
    printf("'" svfmt "'\n", svarg(sv));

    // struct string_view *words = svtoks(sv, " ;");
    // struct string_view *words = svtokv(sv, svfromcstr(" ;"));
    struct string_view *words = svtokc(sv, ';');

    arrforeach(struct string_view, words) {
        printf("[%d] '" svfmt "'\n", arroff(words, it), svarg(*it));
    }
    arrfree(words);

    return 0;
}
