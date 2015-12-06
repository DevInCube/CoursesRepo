#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void){
    // pointers declaration
    char * inputStr     = NULL;
    char * vowelCharPtr = NULL;                              // just pointer to some char of inputStr memory
    char * vowels       = "eyuioaEYUIOA";                    // const string, no need to allocate memory
    int  * count        = NULL;
    
    // memory allocation
    inputStr = (char *) malloc(50 * sizeof(char));           // alloc memory for input string
    count    = (int  *) malloc( 1 * sizeof(int ));           // alloc memory for one int counter of vowels
    if (NULL == inputStr || NULL == count){
        printf("Alloc error");
        return EXIT_FAILURE;
    }
    
    // assignments
    *count = 0;                                              // don't forget to init counter memory
    printf("Please, input string: ");
    scanf("%s", inputStr);
    
    // algorithm
    vowelCharPtr = strpbrk(inputStr, vowels);                // get first occurance of vowel char in inputStr
    while (NULL != vowelCharPtr) {                           // if no vowels, vowelCharPtr would be NULL
        (*count) ++;                                         // increment counter
        vowelCharPtr = strpbrk(vowelCharPtr + 1, vowels);    // get next occurance of vowel char after this occurance
    }
    
    // print result
    printf("Number of vowels: %i", *count);
    
    // memory deallocation
    free(inputStr);         // for malloc
    free(count);            // for malloc
    // free(vowels);        // no need to free this memory.  It points to constant memory of "eyuioaEYUIOA" string literal
    // free(vowelCharPtr);  // no need to free this pointer. It points to memory of inputStr pointer
    
    return EXIT_SUCCESS;
}
