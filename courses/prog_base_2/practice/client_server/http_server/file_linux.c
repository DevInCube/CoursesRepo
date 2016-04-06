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
    fread(*nullBuf, fileLen, 1, file);
    fclose(file);
    return fileLen;
}
