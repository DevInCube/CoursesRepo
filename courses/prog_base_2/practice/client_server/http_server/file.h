#ifndef FILE_H_INCLUDED
#define FILE_H_INCLUDED

int
file_exists(const char * fileName);

int
file_readAllBytes(const char * fileName, char * buff, int maxBuffSize);

#endif // FILE_H_INCLUDED
