#include <stdio.h>
#include <stdlib.h>

#define __LEN(X) sizeof(X)/sizeof(X[0])
#define __BOOLSTR(X) ((X) ? "TRUE" : "FALSE")

struct TestCase {
    const char * input;
    int unit_len;
    int pad_len;
    char * output;
};

int main(void) {
    char outputBuffer[1000];
    struct TestCase testCaseArray[] = {
        {"H", 2, 0, "11001100110011"},
        {"HEY DUDE", 2, 0, "1100110011001100000011000000111111001100111111001111110000000000000011111100110011000000110011001111110000001111110011001100000011"},
        {"HEY DUDE", 1, 3, "00010101010001000111010111011100000001110101000101011100011101010001000"},
        {"HEY DUDE", 3, 0, "111000111000111000111000000000111000000000111111111000111000111111111000111111111000000000000000000000111111111000111000111000000000111000111000111111111000000000111111111000111000111000000000111"},
        {"HELLO KITTY", 2, 2,"0011001100110011000000110000001100111111001100110000001100111111001100110000001111110011111100111111000000000000001111110011001111110000001100110000001111110000001111110000001111110011001111110011111100"},
        {"ONE TWO THREE", 4, 0, "1111111111110000111111111111000011111111111100000000000011111111111100001111000000000000111100000000000000000000000000001111111111110000000000001111000011111111111100001111111111110000000000001111111111110000111111111111000011111111111100000000000000000000000000001111111111110000000000001111000011110000111100001111000000000000111100001111111111110000111100000000000011110000000000001111"}
    };
    char * testOutput = NULL;
    int testCaseArrayLength = __LEN(testCaseArray);
    int testIndex = 0;
    int testStatus = 0;
    struct TestCase * testCase = NULL;

    puts("ENCODE:\n");
    for (testIndex = 0; testIndex < testCaseArrayLength; testIndex++) {
        testCase = &testCaseArray[testIndex];
        outputBuffer[0] = '\0';
        testOutput = morse_encode(outputBuffer, testCase->input, testCase->unit_len, testCase->pad_len);
        testStatus = (0 == strcmp(testOutput, testCase->output));
        printf("%s // %s\n", testCase->input, __BOOLSTR(testStatus));
    }

    puts("\nDECODE:\n");
    for (testIndex = 0; testIndex < testCaseArrayLength; testIndex++) {
        testCase = &testCaseArray[testIndex];
        outputBuffer[0] = '\0';
        testOutput = morse_decode(outputBuffer, testCase->output);
        testStatus = (0 == strcmp(testOutput, testCase->input));
        printf("%s == %s // %s\n", testCase->input, testOutput, __BOOLSTR(testStatus));
    }

    printf("\nEnd.\n");
    return 0;
}
