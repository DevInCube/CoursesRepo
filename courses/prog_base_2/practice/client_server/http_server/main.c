#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>  // ptrdiff_t
#include <signal.h>
#include <errno.h>
#include <unistd.h>

#include "socket.h"
#include "file.h"
#include "student.h"
#include "http.h"
#include "io.h"  // io_kbhit();

#define CRLF "\r\n"

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

static int _send_all(socket_t *, const char *, size_t);

void server_notFound(socket_t * client);

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
        free((void *)allStudentsJson);
    } else if (strcmp(req->method, "POST") == 0) {
        student_t st = student_empty();
        const char * str = http_request_getArg(req, "name");
        if (NULL != str) {
            st.name = (char *)str;
        }
        str = http_request_getArg(req, "surname");
        if (NULL != str) {
            st.surname = (char *)str;
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
        free((void *)jSt);
    } else if (strcmp(req->method, "OPTIONS") == 0) {
































































































































        printf("Options on students\n");
    } else if (strcmp(req->method, "DELETE") == 0) {
        sprintf(strbuf,
            "HTTP/1.1 200 OK\n"
            "\n");
        printf("Delete student\n");
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
    free((void *)studentJson);
    socket_write_string(client, strbuf);
    socket_close(client);
}

void server_file(socket_t * client, const char * fileName) {
    char * fileBuf;
    int fileLen = file_readAllBytes(fileName, &fileBuf);

    char strbuf[10240];
    sprintf(strbuf,
        "HTTP/1.1 200 OK" CRLF
        "Content-Length: %i\n" CRLF
        // "Content-Type: image/png\n"
        "Accept-Ranges: bytes\n" CRLF
        "\n", fileLen);
    socket_write_string(client, strbuf);
    printf(">> Sending file '%s' of size %i bytes...\n\n", fileName, fileLen);
    socket_write(client, fileBuf, fileLen);
    socket_close(client);

    free(fileBuf);
}

void server_file_c(socket_t * client, const char * fileName) {
    /// https://ru.wikipedia.org/wiki/Chunked_transfer_encoding
    /// http://weblog.rubyonrails.org/2011/4/18/why-http-streaming/
    char * fileBuf;
    int fileLen = file_readAllBytes(fileName, &fileBuf);
    int chunksCount = 100;
    int chunkSize = fileLen / chunksCount;
    char header[10240];
    sprintf(header,
        "HTTP/1.1 200 OK" CRLF
        // "Content-Type: image/png\n"
        "Accept-Ranges: bytes" CRLF
        "Transfer-Encoding: chunked" CRLF
        CRLF);
    socket_write_string(client, header);
    for (int i = 0; i < chunksCount + 1; i++) {
        printf(">> Sending file '%s' of size %i bytes...(%i, %i)\n\n",
            fileName, chunkSize, i * chunkSize, (i + 1) * chunkSize);
        char sSizeStr[20] = "";
        sprintf(sSizeStr, "%x\r\n", chunkSize);
        printf("%s<CRLF>\n", sSizeStr);
        socket_write_string(client, sSizeStr);
        _send_all(client, fileBuf + i * chunkSize, chunkSize);
        socket_write_string(client, "\r\n");
        usleep(100*1000);
    }
    socket_write(client, NULL, 0);
    socket_close(client);
    free(fileBuf);
}

void server_live_stream(socket_t * client) {
    const char * boundary = "someimgboundary";
    char header[1024] = "";
    int hsize = sprintf(header,
        "HTTP/1.1 200 OK" CRLF
        "Connection: close" CRLF
        "Cache-control: no-cache, max-age=0" CRLF
        "Content-type: multipart/x-mixed-replace;"
        " boundary=%s" CRLF
        CRLF,
        boundary);
    if (SOCKET_ERR == socket_write(client, header, hsize)) {
        printf("Error writing mixed head\n");
        exit(1);
    }

    const char * files[] = {
        "2.jpg",
        "1.jpg",
        "3.jpg"
    };
    char * streams[] = { NULL, NULL, NULL };
    int streamSizes[] = {0, 0, 0};
    for (int i = 0; i < 3; i ++) {
        streamSizes[i] = file_readAllBytes(files[i], &streams[i]);
    }
    int i = 0;
    char mixedBoundaryHead[200] = "";

    while (1) {
        char * fileStream = streams[i];
        int streamSize = streamSizes[i];

        int mheadSize = sprintf(mixedBoundaryHead,
                CRLF "--%s" CRLF
                "Content-type: image/jpeg" CRLF
                "Content-length: %i" CRLF
                CRLF,
                boundary, streamSize);
        int rc = 0;
        if (SOCKET_ERR == (rc = socket_write(client, mixedBoundaryHead, mheadSize))) {
            printf("Error writing boundary %i | %i\n", rc, errno);
            break;
        }
        printf("Sending image stream [%i bytes]\n", streamSize);
        if (SOCKET_ERR == _send_all(client, fileStream, streamSize)) {
            break;
        }
        usleep(300 * 1000);
        i = (i + 1) % 3;
    }
    for (int i = 0; i < 3; i++) {
        free(streams[i]);
    }
    socket_close(client);
}

void server_live(socket_t * client) {
    char replyBuf[1024];
    const char * htmlText = "<img src='/live-stream' alt='LIve video'>";
    sprintf(replyBuf,
        "HTTP/1.1 200 \n"
        "Content-Type: text/html\n"
        "Content-Length: %zu\n"
        "\n%s", strlen(htmlText), htmlText);
    socket_write_string(client, replyBuf);
    socket_close(client);
}

void server_video(socket_t * client, const char * fileName) {
    FILE * video = NULL;
    int chunkSize = 0;
    char buff[1024] = "";
    char xChunkSizeStr[32] = "";
    sprintf(buff,
        "HTTP/1.1 200 OK\n"
        "Content-Type: video/mp4\n"
        "Accept-Ranges: bytes\n"
        "Transfer-Encoding: chunked\n"
        "\n");
    socket_write_string(client, buff);
    video = fopen(fileName, "r");
    while ((chunkSize = fread(buff, 1, sizeof(buff), video)) != 0) {
        printf("Read chunk of %i bytes\n", chunkSize);
        sprintf(xChunkSizeStr,
            "%x" CRLF,
            chunkSize);
        socket_write_string(client, xChunkSizeStr);
        _send_all(client, buff, chunkSize);
        socket_write_string(client, CRLF);
    }
    socket_write_string(client, "0" CRLF CRLF);
    socket_close(client);
    fclose(video);
}

static int _send_all(socket_t * client, const char * msg, size_t msglen) {
    while (msglen > 0) {
         int len = socket_write(client, msg, msglen);
         if (SOCKET_ERR == len) {
            return SOCKET_ERR;
         }
         msg += len;
         msglen -= len;
    }
    return 0;
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

static socket_t * g_serverSock;

void cleanup_server(void);
void inthandler(int);

int main(void) {
    int sPort = 5000;

    atexit(cleanup_server);
    signal(SIGINT, inthandler);

    lib_init();
    g_serverSock = socket_new();
    while (1) {
        if (socket_bind(g_serverSock, sPort) == SOCKET_ERR) {
            printf("Port %i can't be binded\n", sPort);
            sPort++;
            //exit(1);
        } else {
            printf("Binded on port %i\n", sPort);
            break;
        }
    }
    socket_listen(g_serverSock);

    char buf[10000];
    socket_t * client = NULL;
    while (1) {
        printf("Accepting clients...\n");
        client = socket_accept(g_serverSock);
        if (NULL == client) {
            printf("NULL client\n");
            exit(1);
        }
        int readStatus = socket_read(client, buf, sizeof(buf));
        if (0 >= readStatus) {
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
        } else if (strcmp(request.uri, "/live") == 0) {
            server_live(client);
        } else if (strcmp(request.uri, "/live-stream") == 0) {
            server_live_stream(client);
        } else {
            const char * filePath = strstr(request.uri, "/") + 1;
            if (file_exists(filePath)) {
                server_video(client, filePath);
            } else {
                server_notFound(client);
            }
        }
        socket_free(client);
    }
    socket_free(g_serverSock);
    lib_free();
    return 0;
}

void inthandler(int sig) {
    exit(0);  // call to cleanup_server at exit
}

void cleanup_server(void) {
    printf("Cleanup server.\n");
    socket_close(g_serverSock);
    socket_free(g_serverSock);
}
