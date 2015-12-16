#include <stdio.h>
#include <stdlib.h>

#include "ns.h"

#define __LEN(X) sizeof(X)/sizeof(X[0])

struct TestCase {
    char * sourceNumber;
    unsigned int sourceBase;
    char * destNumber;
    unsigned int destBase;
};

int compare(char * n1, char * n2) {
    int i;
    for (i = 0; n1[i] != '\0' && n1[i] != '.'; i++)
        ;
    if (n1[i] == '.') {
        i += 2;  // accuracy of fraction
    }
    return 0 == strncmp(n1, n2, i);
}

int main()
{
    struct TestCase testCases[] = {
        {"0", 15, "0", 27},
        {"3", 10, "11", 2},
        {"F", 16, "15", 10},
        {"5.5", 10, "12.11111", 3},
        {"125.125", 6, "311.03323", 4},
        {"125.125", 6, "53.24537", 10},
        {"53.24537", 10, "125.125", 6},
        {"125.125", 6, "49.27765", 11},
        {"125.125", 6, "32.42F89", 17},
        {"125.125", 6, "1P.6OAAA", 28},
        {"53.24537", 10, "125.125", 6},
        {"10101.111", 10, "7SL.3ZUTD", 36},
    };

    int lenCases = __LEN(testCases);
    int i;
    char sourceNumber[1000];
    char * res = NULL;
    struct TestCase * tCase = NULL;

    puts("ENCODE:\n");
    for (i = 0; i < lenCases; i++) {
        tCase = &testCases[i];
        strcpy(sourceNumber, tCase->sourceNumber);
        printf("%s(%i) = ", tCase->sourceNumber, tCase->sourceBase);
        res = ns_convert(sourceNumber, tCase->sourceBase, tCase->destBase);
        printf("{%s(%i)} == {%s} // %s\n", tCase->destNumber, tCase->destBase, res, compare(res, tCase->destNumber) ? "TRUE" : "FALSE" );
    }
    return 0;
}
