#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>  // ptrdiff_t

#include "socket.h"
#include "file.h"

const char * METHODS[] = {"GET", "POST", "PUT", "DELETE"};
typedef enum {
    HTTP_GET, HTTP_POST, HTTP_PUT, HTTP_DELETE
} HTTP_METHOD;

int http_getPath(const char * const request, char * pathBuf, int maxPathBuf) {
    // get method
    char method[8];
    ptrdiff_t methodLen = strstr(request, " ") - request;  // find first whitespace
    memcpy(method, request, methodLen);
    method[methodLen] = '\0';
    printf("METHOD: %s\n", method);
    // get uri
    const char * uriStartPtr = request + strlen(method) + 1;
    const char * uriEndPtr = strstr(uriStartPtr, " ");  // find second whitespace
    ptrdiff_t uriLen = uriEndPtr - uriStartPtr;
    memcpy(pathBuf, uriStartPtr, uriLen);
    pathBuf[uriLen] = '\0';
    printf("URI: %s\n", pathBuf);
    return uriLen;
}

void server_homepage(socket_t * client) {
    char homeBuf[10240];
    const char * htmlText =
        "<head>"
            "<meta charset=\"utf-8\"/>"
            "<title>C Server</title></head>"
        "<body><h1>oh...my...GOD!</h1><img src='image.png'/></body>";
    sprintf(homeBuf,
        "HTTP/1.1 200 OK\n"
        "Content-Type: text/html\n"
        "Content-Length: %zu\n"
        "Connection: keep-alive\n"
        "\n%s", strlen(htmlText), htmlText);
    socket_write_string(client, homeBuf);
    socket_close(client);
}

void server_file(socket_t * client, const char * fileName) {
    char * fileBuf;
    int fileLen = file_readAllBytes(fileName, &fileBuf);
    char header[10240];
    sprintf(header,
        "HTTP/1.1 200 OK\n"
        "Content-Length: %i\n"
        // "Content-Type: image/png\n"
        "Accept-Ranges: bytes\n"
        "Connection: close\n"
        "\n", fileLen);
    socket_write_string(client, header);
    printf(">> Sending file '%s' of size %i bytes...\n\n", fileName, fileLen);
    socket_write(client, fileBuf, fileLen);
    socket_close(client);
    free(fileBuf);
}

void server_notFound(socket_t * client) {
    socket_write_string(client, "HTTP/1.1 404 \n");
    socket_close(client);
}

int main() {
    lib_init();
    socket_t * server = socket_new();
    socket_bind(server, 5000);
    socket_listen(server);

    char buf[10000];
    char pathBuf[256];
    socket_t * client = NULL;
    while(1) {
        client = socket_accept(server);
        socket_read(client, buf, sizeof(buf));
        printf(">> Got request:\n%s\n", buf);

        http_getPath(buf, pathBuf, sizeof(pathBuf));

        if (strcmp(pathBuf, "/") == 0) {
            server_homepage(client);
        } else {
            const char * filePath = strstr(pathBuf, "/") + 1;
            if (file_exists(filePath)) {
                server_file(client, filePath);
            } else {
                server_notFound(client);
            }
        }
    }
    socket_free(server);
    lib_free();
    return 0;
}
