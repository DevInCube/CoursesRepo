#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>  // ptrdiff_t

#include "socket.h"
#include "file.h"
#include "student.h"
#include "http.h"

const char * METHODS[] = {"GET", "POST", "PUT", "DELETE"};
typedef enum {
    HTTP_GET, HTTP_POST, HTTP_PUT, HTTP_DELETE
} HTTP_METHOD;

student_t g_students[] = {
    {
        .name = "Ira",
		.surname = "Ogirok",
		.year = 1992,
		.score = 3.92,
		.group = {
			.name = "KP-91",
			.faculty = "FAKS",
		}
    },
    {
        .name = "Taras",
		.surname = "Kartoplia",
		.year = 1990,
		.score = 4.9,
		.group = {
			.name = "KP-51",
			.faculty = "PBF",
		}
    }
};


void server_homepage(socket_t * client) {
    char homeBuf[10240];
    const char * htmlText =
        "<head>"
            "<meta charset=\"utf-8\"/>"
            "<title>C Server</title></head>"
        "<body>"
            "<h1>oh...my...GOD!</h1>"
            "<img src='image.png'/>"
            "<div><a href='/students'>All students</a></div>"
        "</body>";
    sprintf(homeBuf,
        "HTTP/1.1 200 OK\n"
        "Content-Type: text/html\n"
        "Content-Length: %zu\n"
        "Connection: keep-alive\n"
        "\n%s", strlen(htmlText), htmlText);
    socket_write_string(client, homeBuf);
    socket_close(client);
}
/*
void server_homepage(socket_t * client) {
    const char * pageText = "Hello, world of web!\n";
    char homeBuf[1024];
    sprintf(homeBuf,
        "HTTP/1.1 200 OK\n"
        "Content-Type: text/html\n"
        "Content-Length: %zu\n"
        "\n%s", strlen(pageText), pageText);
    socket_write_string(client, homeBuf);
    socket_close(client);
}
*/
void server_students(socket_t * client, http_request_t * req) {
    char strbuf[10240];
    if (strcmp(req->method, "GET") == 0) {
        const char * allStudentsJson = student_listToJSON(g_students, 2);
        sprintf(strbuf,
            "HTTP/1.1 200 OK\n"
            "Content-Type: application/json\n"
            "Content-Length: %zu\n"
            "Connection: keep-alive\n"
            "\n%s", strlen(allStudentsJson), allStudentsJson);
        free(allStudentsJson);
    } else if (strcmp(req->method, "POST") == 0) {
        student_t st = student_empty();
        const char * str = http_request_getArg(req, "name");
        if (NULL != str) {
            st.name = str;
        }
        str = http_request_getArg(req, "surname");
        if (NULL != str) {
            st.surname = str;
        }
        str = http_request_getArg(req, "year");
        if (NULL != str) {
            st.year = atoi(str);
        }
        const char * jSt = student_toJSON(&st);
        printf("New student:\n%s\n", jSt);
        sprintf(strbuf,
            "HTTP/1.1 200 OK\n"
            "Content-Type: application/json\n"
            "Content-Length: %zu\n"
            "Connection: keep-alive\n"
            "\n%s", strlen(jSt), jSt);
        free(jSt);
    }
    socket_write_string(client, strbuf);
    socket_close(client);
}

void server_studentById(socket_t * client, const char * pathBuf) {
    const char * pattern = "/students/";
    char * c = NULL;
    long studentIndex = strtol(pathBuf + strlen(pattern), &c, 10);
    if (studentIndex < 0 || studentIndex >= 2) {
        server_notFound(client);
        return;
    }
    char strbuf[10240];
    const char * studentJson = student_toJSON(&g_students[studentIndex]);
    sprintf(strbuf,
        "HTTP/1.1 200 OK\n"
        "Content-Type: application/json\n"
        "Content-Length: %zu\n"
        "Connection: keep-alive\n"
        "\n%s", strlen(studentJson), studentJson);
    free(studentJson);
    socket_write_string(client, strbuf);
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
    char replyBuf[1024];
    const char * htmlText = "<h1>404 Page Not Found!</h1>";
    sprintf(replyBuf,
        "HTTP/1.1 404 \n"
        "Content-Type: text/html\n"
        "Content-Length: %zu\n"
        "\n%s", strlen(htmlText), htmlText);
    socket_write_string(client, replyBuf);
    socket_close(client);
}

int main() {
    lib_init();
    socket_t * server = socket_new();
    socket_bind(server, 5002);
    socket_listen(server);

    char buf[10000];
    socket_t * client = NULL;
    while(1) {
        client = socket_accept(server);
        if (NULL == client) {
            printf("NULL client\n");
            exit(1);
        }
        int readStatus = socket_read(client, buf, sizeof(buf));
        if (0 == readStatus) {
            printf("Skipping empty request.\n");
            socket_close(client);
            socket_free(client);
            continue;
        }
        printf(">> Got request (read %i):\n`%s`\n", readStatus, buf);

        http_request_t request = http_request_parse(buf);

        if (strcmp(request.uri, "/") == 0) {
            server_homepage(client);
        } else if (strcmp(request.uri, "/students") == 0) {
            server_students(client, &request);
        } else if (strstr(request.uri, "/students/") == request.uri) {
            server_studentById(client, request.uri);
        } else {
            const char * filePath = strstr(request.uri, "/") + 1;
            if (file_exists(filePath)) {
                server_file(client, filePath);
            } else {
                server_notFound(client);
            }
        }
        socket_free(client);
    }
    socket_free(server);
    lib_free();
    return 0;
}
