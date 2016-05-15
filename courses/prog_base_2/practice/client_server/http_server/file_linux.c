#include <stdlib.h>
#include <stdio.h>

#include <unistd.h>

#define FILE_ERROR -1

int
file_exists(const char * fileName) {
    return (access(fileName, F_OK ) != -1);
}

int
file_readAllBytes(const char * fileName, char ** const nullBuf) {
    FILE * file;
    file = fopen(fileName, "rb");
    if (!file) {
        return FILE_ERROR;
    }
    fseek(file, 0, SEEK_END);
    int fileLen = ftell(file);
    *nullBuf = malloc(sizeof(char) * fileLen);
    fseek(file, 0, SEEK_SET);
    int r = fread(*nullBuf, 1, fileLen, file);
    if (r != fileLen) {
        printf("File %s read err %i != %i\n", fileName, r, fileLen);
        exit(1);
    }
    fclose(file);
    return fileLen;
}
