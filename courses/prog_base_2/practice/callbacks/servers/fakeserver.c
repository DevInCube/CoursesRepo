#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <time.h>   // nanosleep

#include "fakeserver.h"
#include "thread.h"

// private types
typedef struct ThreadArgs {
    Server_T server;
    int cancel;
} ThreadArgs;

struct Server_T {
    const char * serverName;
    Server_Events_T events;
    void * userData;
    Thread_T thread;
    ThreadArgs * targs;
};

// private prototypes
static void * _Thread_run(void * _args);

// constructor
Server_T Server_new(const char * serverName, Server_Events_T events, void * userData) {
    Server_T s = (Server_T)malloc(sizeof(struct Server_T));
    if (NULL == s) {
        printf("Can't alloc memory for server\n");
        exit(1);
    }
    s->serverName = serverName;
    s->events = events;
    s->userData = userData;
    s->thread = NULL;
    s->targs = NULL;
    return s;
}

// API functions implementations

const char * Server_getServerName(Server_T server) {
    return server->serverName;
}

void * Server_getUserData(Server_T server) {
    return server->userData;
}

void Server_start(Server_T s) {
    ThreadArgs * targs = (ThreadArgs *)malloc(sizeof(ThreadArgs));
    targs->server = s;
    targs->cancel = 0;
    s->targs = targs;
    s->thread = Thread_create_args(_Thread_run, s->targs);
}

void Server_stop(Server_T s) {
    s->targs->cancel = 1;       // set cancel thread loop flag
    Thread_join(s->thread);     // wait for complete
    Thread_delete(s->thread);
}

// destructor
void Server_delete(Server_T s) {
    free(s->targs);
    free(s);
}

// PRIVATE functions

static void * _Thread_run(void * args) {
    ThreadArgs * targs = (ThreadArgs *)args;
    Server_T server = targs->server;
    Server_Events_T events = server->events;
    int code;
    const struct timespec t = {0, 50000000L};     // { sec, nanosec }
    if (events != NULL && events->stop != NULL) {
        events->start(server, NULL);     // on start event callback
    }
    while (!targs->cancel) {
        code = rand() % 30;               // some random event
        if (code < 10) {
            if (events != NULL && events->message != NULL) {
                events->message(server, (void *)&code);     // message callback
            }
        }
        nanosleep(&t, NULL);
    }
    if (events != NULL && events->stop != NULL) {
        events->stop(server, NULL);      // on stop event callback
    }
    return NULL;
}
