#include <stdlib.h>
#include <stdio.h>

char * process(char * resultStr, const char * textLines[],
               int linesNum, const char * extraStr);

int main(void) {
    char result[100];
    char *text[] = {
        "This text",
        "is already",
        "divided into",
        "lines."
    };
    int linesNum = sizeof(text) / sizeof(text[0]);
    char extra[100] = "some extra line";
    char * res = process(result, text, linesNum, extra);
    puts("=== Result ===");
    puts(res);
}
