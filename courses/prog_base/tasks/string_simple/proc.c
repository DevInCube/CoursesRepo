#include <string.h>

char * process(char * resultStr, const char * textLines[],
               int linesNum, const char * extraStr) {
    int i;
    char * line;
    puts("=== Text ===");
    for(i = 0; i < linesNum; i++) {
        line = textLines[i];
        puts(line);
    }
    puts("=== Extra string ===");
    puts(extraStr);
    strcpy(resultStr, "This is result: ");
    strcat(resultStr, extraStr);
    return resultStr;
}

