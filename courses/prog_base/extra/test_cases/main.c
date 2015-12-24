#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

/* some helping macros */
#define __LEN(X) sizeof(X) / sizeof(X[0])
#define __STRBOOL(B) ((B) ? "true" : "false")
#define __STR(X) #X

#define ERROR -1

/*
count number of vowels in interval [start..end] of str
returns ERROR on error
*/
int func(const char * str, int start, int end);
/*
function with test code
*/
void testFunc(void);

int main(void)
{
    testFunc();
    return 0;
}

int func(const char * str, int start, int end) {
    static const char VOWELS[] = "EYUIOAeyuioa";
    const int len = strlen(str);
    int i, result = 0;
    if (end < start || start < 0 || start >= len || end >= len) {
        return ERROR;  /* invalid interval */
    }
    for (i = start; i <= end; i++) {
        if (NULL != strchr(VOWELS, str[i])) {
            result++;
        }
    }
    return result;
}

void testFunc(void) {
    struct {
        const char * const str;
        const int start;
        const int end;
        const int result;
    } cases[] = {
        /* error test cases */
        {"google", -1, 0, ERROR},
        {"google", 2, 0, ERROR},
        {"google", 0, 6, ERROR},
        /* empty result cases */
        {"google", 0, 0, 0},
        {"google", 3, 4, 0},
        /* positive result cases */
        {"goOgle", 0, 5, 3},
        {"goOgle", 1, 1, 1},
        {"goOgle", 1, 2, 2},
        /* all vowels case */
        {"EYUIOAeyuioa", 0, 11, 12}
    }, *pCase = NULL;
    const int casesNum = __LEN(cases);
    int i, result, equals, passedCount = 0;

    printf("Test function '%s':\n", __STR(func));
    for (i = 0; i < casesNum; i++) {
        pCase = &cases[i];
        result = func(pCase->str, pCase->start, pCase->end);
        equals = (pCase->result == result);
        passedCount += equals;
        printf("\tcase %i // %s\n", i, __STRBOOL(equals));
    }
    printf("Test result: passed %i of %i\n", passedCount, casesNum);
}
