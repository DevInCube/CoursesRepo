#include <stdio.h>
#include <stdlib.h>
#include <stddef.h> //  ptrdiff_t
#include <string.h>

#include "socket.h"

typedef struct {
    char key[256];
    char value[256];
} keyvalue_t;

typedef struct {
    char method[8];
    char uri[256];
    keyvalue_t * form;
    int formLength;
} http_request_t;

http_request_t
http_request_parse(const char * const request);

const char *
http_request_getArg(http_request_t * self, const char * key);

const char *
keyvalue_toString(keyvalue_t * self);

int main() {
    lib_init();

    socket_t * serverSocket = socket_new();
    socket_bind(serverSocket, 5009);
    socket_listen(serverSocket);

    while (1) {
        puts("Waiting for client...");
        socket_t * clientSocket = socket_accept(serverSocket);

        char buf[100000];
        int readLen = socket_read(clientSocket, buf, 100000);

        if (0 == readLen) {
            puts("Skipping empty request");
            socket_close(clientSocket);
            socket_free(clientSocket);
            continue;
        }

        printf("Got request:\n--------------\n%s\n-----------------\n", buf);

        http_request_t req = http_request_parse(buf);

        if (strcmp(req.uri, "/") == 0) {
            socket_write_string(clientSocket, "Hello, World!\n");
        } else if (strcmp(req.uri, "/students") == 0) {

            /*if (strcmp(req.method, "GET") == 0) {
                socket_write_string(clientSocket, "List of students\n");
            } else*/
            if (strcmp(req.method, "GET") == 0) {

                char * stName = "Goog";//http_request_getArg(&req, "name");
                char * stSurname = "Gleee"; //http_request_getArg(&req, "surname");

                char res[1000];
                int ll = sprintf(res, "{ \"name\": \"%s\", \"surname\": \"%s\" }\n\0",
                        stName, stSurname
                    );
                printf("RES:\n%s\nll: %i\nstrlen: %i\n", res, ll, strlen(res));
                char result[1000];
                sprintf(result,
                    "HTTP/1.1 200 OK\n"
                    "Content-length: %i\n"
                    "Content-type: application/json\n"
                    "\n"
                    "%s\0",
                    ll, res
                );
                printf("RESULT:\n%s\n", result);
                socket_write_string(clientSocket, result);

            } else {
                socket_write_string(clientSocket, "Method not allowed!\n");
            }

        } else {
            socket_write_string(clientSocket, "Not found!\n");
        }

        socket_close(clientSocket);
        socket_free(clientSocket);
    }

    socket_close(serverSocket);
    socket_free(serverSocket);

    lib_free();
    return 0;
}





http_request_t
http_request_parse(const char * const request) {
    http_request_t req;
    req.form = NULL;
    req.formLength = 0;
    // get method
    ptrdiff_t methodLen = strstr(request, " ") - request;  // find first whitespace
    memcpy(req.method, request, methodLen);
    req.method[methodLen] = '\0';
    // get uri
    const char * uriStartPtr = request + strlen(req.method) + 1;
    const char * uriEndPtr = strstr(uriStartPtr, " ");  // find second whitespace
    ptrdiff_t uriLen = uriEndPtr - uriStartPtr;
    memcpy(req.uri, uriStartPtr, uriLen);
    req.uri[uriLen] = '\0';
    // parse form data
    const char * bodyStartPtr = strstr(request, "\r\n\r\n") + strlen("\r\n\r\n");
    const char * cur = bodyStartPtr;
    const char * pairEndPtr = cur;
    const char * eqPtr = cur;
    while (strlen(cur) > 0) {
        pairEndPtr = strchr(cur, '&');
        if (NULL == pairEndPtr) {
            pairEndPtr = cur + strlen(cur);
        }
        keyvalue_t kv;
        // get key
        eqPtr = strchr(cur, '=');
        ptrdiff_t keyLen = eqPtr - cur;
        memcpy(kv.key, cur, keyLen);
        kv.key[keyLen] = '\0';
        // get value
        eqPtr++;
        ptrdiff_t valueLen = pairEndPtr - eqPtr;
        memcpy(kv.value, eqPtr, valueLen);
        kv.value[valueLen] = '\0';
        // insert key-value pair into request form list
        req.formLength += 1;
        req.form = realloc(req.form, sizeof(keyvalue_t) * req.formLength);
        req.form[req.formLength - 1] = kv;
        cur = pairEndPtr + ((strlen(pairEndPtr) > 0) ? 1 : 0);
    }
    return req;
}

const char *
http_request_getArg(http_request_t * self, const char * key) {
    for (int i = 0; i < self->formLength; i++) {
        if (strcmp(self->form[i].key, key) == 0) {
            return self->form[i].value;
        }
    }
    return NULL;
}

const char * keyvalue_toString(keyvalue_t * self) {
    char * str = malloc(sizeof(char) * (strlen(self->key) + strlen(self->value) + 2));
    sprintf(str, "%s=%s\0", self->key, self->value);
    return str;
}







/*
int main() {
    lib_init();
    socket_t * serverSocket = socket_new();
    socket_bind(serverSocket, 5001);
    socket_listen(serverSocket);

    while (1) {
        puts("Waiting for connections...");
        socket_t * clientSocket = socket_accept(serverSocket);
        puts("New client");
        char buf[102400];
        int readLength = socket_read(clientSocket, buf, 102400);

        if(readLength == 0) {
            socket_close(clientSocket);
            socket_free(clientSocket);
            puts("Skipping empty request");
            continue;
        }

        http_request_t req = http_request_parse(buf);
        printf("Method: %s\nURI: %s\n", req.method, req.uri);
        for(int i = 0; i < req.formLength; i ++) {
            const * kvStr = keyvalue_toString(&req.form[i]);
            printf("%s\n", kvStr);
            free(kvStr);
        }

        printf("Client sent:\n--------\n%s\n---------\n", buf);

        if (strcmp(req.uri, "/") == 0) {
            socket_write_string(clientSocket, "Hello, World!");
        } else if (strcmp(req.uri, "/students") == 0) {
            if (strcmp(req.method, "GET") == 0) {
                socket_write_string(clientSocket, "List of students");
            } else if (strcmp(req.method, "POST") == 0) {


                const char * stName = http_request_getArg(&req, "name");
                const char * stSurname = http_request_getArg(&req, "surname");

                printf("New student:\nName: %s\nSurname: %s\n\n", stName, stSurname);

                char res[1000];
                char * json = "{ \"name\": \"Rost\", \"surname\": \"Google\" }";
                sprintf(res,
                    "HTTP/1.1 200 OK\n"
                    "Content-length: %zu\n"
                    "Content-type: application/json\n"
                    "\n"
                    "%s", strlen(json), json
                );
                socket_write_string(clientSocket, res);

            } else {
                socket_write_string(clientSocket, "Not allowed");
            }
        } else {
            socket_write_string(clientSocket, "Not found!");
        }

        socket_close(clientSocket);
        socket_free(clientSocket);
    }

    socket_close(serverSocket);
    socket_free(serverSocket);
    lib_free();
    return 0;
}*/
