#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
//#inlcude <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <string.h>
#include <time.h>

#include "socket.h"

struct socket_s {
    int fd;
    struct sockaddr_in addr;
};

socket_t * socket_new(void) {
    socket_t * self = malloc(sizeof(struct socket_s));
    self->fd = socket(AF_INET, SOCK_STREAM, 0);
    return self;
}

void socket_free(socket_t * self) {
    free(self);
}

const char * socket_getIPAddress(socket_t * self) {
    return inet_ntoa(self->addr.sin_addr);
}

int socket_getPortNumber(socket_t * self) {
    return (int) ntohs(self->addr.sin_port);
}

int socket_bind(socket_t * self, int portNumber) {
    struct sockaddr_in serv_addr;

    memset(&serv_addr, '0', sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(portNumber);

    self->addr = serv_addr;
    int status = bind(self->fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    return status;
}

int socket_listen(socket_t * self) {
    const int MAX_QUEUE_CLIENTS = 10;
    int status = listen(self->fd, MAX_QUEUE_CLIENTS);
    //printf("socket listen.\n");
    return status;
}

int socket_connect(socket_t * sock, const char * ipaddress, int portNumber) {
    struct sockaddr_in serv_addr;
    memset(&serv_addr, '0', sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(portNumber);
    if (inet_pton(AF_INET, ipaddress, &serv_addr.sin_addr) <= 0) {
        return -1; //@todo
    }
    int res = connect(sock->fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)); // < 0 - error
    return res;
}

socket_t * socket_accept(socket_t * socket) {
    struct sockaddr_in client_addr;
    socklen_t clilen = sizeof(client_addr);
    socket_t * conn = (socket_t *) malloc(sizeof(struct _socket_t *));
    conn->fd = accept(socket->fd, (struct sockaddr *)&client_addr, &clilen);
    conn->addr = client_addr;
    //printf("Connection accepted [%i].\n", conn->fd);
    //printf("IP address is: %s\n", inet_ntoa(client_addr.sin_addr));
    //printf("Port is: %d\n", (int) ntohs(client_addr.sin_port));
    return conn;
}

int socket_read(socket_t * self, char * recvBuff, int recvSize) {
    int nRead = read(self->fd, recvBuff, recvSize - 1);
    recvBuff[nRead] = '\0';
    return nRead;
}

int socket_write(socket_t * self, const char * msg, int msgLen) {
    int nWritten = write(self->fd, msg, msgLen);
    return nWritten;
}

int socket_write_string(socket_t * self, const char * str) {
    return socket_write(self, str, strlen(str));
}

void socket_close(socket_t * self) {
    int status = close(self->fd);
    if (0 != status) {
        // @todo
    }
}

/* STATIC */

void lib_init(void) { }
void lib_free(void) { }
