#include <stdio.h>
#include <stdlib.h>

#include <windows.h>
#include <winsock2.h>

#define LEN(ARR) (sizeof(ARR) / sizeof(ARR[0]))

typedef struct list_s list_t;

list_t * list_new(void);
void list_free(list_t * self);
void list_addLast(list_t * self, void * data);
void * list_remove(list_t * self, int index);
void * list_get(list_t * self, int index);
int list_findIndex(list_t * self, void * data);
int list_getSize(list_t * self);

int main() {
    WSADATA wsa;
    WSAStartup(MAKEWORD(2, 2), &wsa);

    int nPort = 5000;
    SOCKET serverSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (INVALID_SOCKET == serverSock) {
        printf("Invalid socket!\n");
        return 1;
    }

    SOCKADDR_IN serverSockAddr;
    serverSockAddr.sin_port = htons(nPort);
    serverSockAddr.sin_family = AF_INET;
    serverSockAddr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(serverSock, (LPSOCKADDR)&serverSockAddr, sizeof(serverSockAddr)) == SOCKET_ERROR) {
        printf("Cannot bind to port %i\n", nPort);
        closesocket(serverSock);
        WSACleanup();
        return 1;
    }

    u_long iMode = 1;
    int iResult = ioctlsocket(serverSock, FIONBIO, &iMode);
    if (iResult != NO_ERROR) {
        printf("ioctlsocket failed with error: %ld\n", iResult);
        closesocket(serverSock);
        WSACleanup();
        return 1;
    }

    if (listen(serverSock, SOMAXCONN) == SOCKET_ERROR) {
        printf("socket listen error %i\n", WSAGetLastError());
        closesocket(serverSock);
        WSACleanup();
        return 1;
    }

    //int clientsSize = 0;
    //SOCKET clients[10] = {SOCKET_ERROR};
    list_t * clientList = list_new();

    char szIncoming[1024];

    fd_set readSet;
    const struct timeval noTimeout = { .tv_sec = 0, .tv_usec = 0 };

    printf("Async server started!");
    while (1) {
        // check for new clients
        FD_ZERO(&readSet);
        FD_SET(serverSock, &readSet);
        int selectStatus = SOCKET_ERROR;
        if ((selectStatus = select(0, &readSet, NULL, NULL, &noTimeout)) == SOCKET_ERROR) {
            printf("Select error %i\n", WSAGetLastError());
            break;
        } else if (selectStatus > 0) {
            struct sockaddr sockAddrClient;
            int size = sizeof(sockAddrClient);
            SOCKET clientSocket = accept(serverSock, &sockAddrClient, &size);
            if (clientSocket != SOCKET_ERROR) {
                SOCKET * pSocket = malloc(sizeof(SOCKET));
                *pSocket = clientSocket;
                list_addLast(clientList, pSocket);
                struct sockaddr_in * saddr = (struct sockaddr_in *)&sockAddrClient;
                char * connected_ip = inet_ntoa(saddr->sin_addr);
                int port = ntohs(saddr->sin_port);
                printf("New client accepted from %s:%i\n", connected_ip, port);
            } else {
                if (WSAGetLastError() != WSAEWOULDBLOCK) {
                    printf("Client accept error!\n");
                    break;
                }
            }
        }

        // check if clients sent something
        if (list_getSize(clientList) == 0) {
            continue;
        }

        FD_ZERO(&readSet);
        for (int i = 0; i < list_getSize(clientList); i++) {
            SOCKET client = *(SOCKET *)list_get(clientList, i);
            FD_SET(client, &readSet);
        }

        int readSelectStatus = SOCKET_ERROR;
        if ((readSelectStatus = select(0, &readSet, NULL, NULL, &noTimeout)) == SOCKET_ERROR) {
            printf("Read select error %i\n", WSAGetLastError());
            break;
        } else if (readSelectStatus > 0) {
            for (int i = 0; i < list_getSize(clientList); i++) {
                SOCKET client = *(SOCKET *)list_get(clientList, i);
                if (FD_ISSET(client, &readSet)) {
                    ZeroMemory(szIncoming, sizeof(szIncoming));
                    int inDataLength = recv(client, (char *)szIncoming, LEN(szIncoming), 0);

                    if(inDataLength == SOCKET_ERROR) {
                        SOCKET * pSocket = list_remove(clientList, i);
                        free(pSocket);
                        i--;
                        printf("Client was disconnected!\n");
                        printf("Total clients: %i\n", list_getSize(clientList));
                    } else {
                        printf("Client[%i] sent: %s\n", i, szIncoming);
                    }
                }
            }
        }
    }
    list_free(clientList);  // @todo close and free clients
    shutdown(serverSock, SD_BOTH);
    closesocket(serverSock);
    WSACleanup();
    return 0;
}

// ======================

typedef struct node_s node_t;

struct node_s {
    node_t * next;
    node_t * prev;
    void * data;
};

static node_t * node_new(void * data);
static void node_free(node_t * self);

struct list_s {
    node_t * head;
    node_t * tail;
    int size;
};

list_t * list_new(void) {
    list_t * self = malloc(sizeof(struct list_s));
    self->size = 0;
    self->head = node_new(NULL);
    self->tail = node_new(NULL);
    self->head->next = self->tail;
    self->tail->prev = self->head;
    return self;
}
void list_free(list_t * self) {
    while (self->size > 0) {
        (void)list_remove(self, 0);
    }
    free(self);
}

void list_addLast(list_t * self, void * data) {
    node_t * newNode = node_new(data);
    newNode->prev = self->tail->prev;
    newNode->prev->next = newNode;
    self->tail->prev = newNode;
    newNode->next = self->tail;
    self->size++;
}
int list_findIndex(list_t * self, void * data) {
    int i = 0;
    for (node_t * cur = self->head->next; cur != self->tail; cur = cur->next) {
        if (cur->data == data) {
            return i;
        }
        i++;
    }
    return -1;
}
void * list_remove(list_t * self, int index) {
    if (index < 0 || index >= self->size) {
        return NULL;
    }
    void * res = NULL;
    node_t * cur = self->head;
    for (int i = 0; (cur->next != self->tail) && (i != index); i++) {
        cur = cur->next;
    }
    node_t * delNode = cur->next;
    res = delNode->data;
    cur->next->next->prev = cur;
    cur->next = cur->next->next;
    self->size--;
    node_free(delNode);
    return res;
}

void * list_get(list_t * self, int index) {
    if (index < 0 || index >= self->size) {
        return NULL;
    }
    node_t * cur = self->head;
    for (int i = 0; (cur->next != self->tail) && (i != index); i++) {
        cur = cur->next;
    }
    return cur->next->data;
}

int list_getSize(list_t * self) {
    return self->size;
}

static node_t * node_new(void * data) {
    node_t * self = malloc(sizeof(struct node_s));
    self->next = NULL;
    self->prev = NULL;
    self->data = data;
    return self;
}

static void node_free(node_t * self) {
    free(self);
}
