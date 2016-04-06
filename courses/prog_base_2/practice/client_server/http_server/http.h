#ifndef HTTP_H_INCLUDED
#define HTTP_H_INCLUDED

typedef struct {
    char method[8];
    char uri[256];
} http_request_t;

#endif // HTTP_H_INCLUDED
