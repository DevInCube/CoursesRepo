#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#include "fakeserver.h"

#define __LEN(X) sizeof(X) / sizeof(X[0])

void onStart(Server_T sender, void * args)  {
    printf(">> Server %s just STARTED!\n", Server_getServerName(sender));
}

void onStop(Server_T sender, void * args)  {
    printf(">> Server %s just STOPPED!\n", Server_getServerName(sender));
    int * userData = (int *)Server_getUserData(sender);
    *userData -= 1;
}

void onMessage(Server_T sender, void * args) {
    int code = *(int *)args;
    const char * sname = Server_getServerName(sender);
    printf("%20s server sent message with code: %i\n", sname, code);
    if (code == 0) {
        printf("Requesting %s server to stop...\n", sname);
        Server_stop(sender);
    }
}

int main(void) {
    int runningServers = 0;
    srand(time(NULL));

    Server_Events_T serverEvents = moduleType_alloc(Server_Events_T);
    serverEvents->message = onMessage;
    serverEvents->start = onStart;
    serverEvents->stop = onStop;

    Server_T servers[] = {
        Server_new("Google", serverEvents, &runningServers),
        Server_new("Microsoft", serverEvents, &runningServers),
        Server_new("Facebook", serverEvents, &runningServers)
    };
    int countServers = __LEN(servers);
    runningServers = countServers;

    for (int i = 0; i < countServers; i++) {
        Server_start(servers[i]);
    }

    while (runningServers != 0) {  // create main loop
        sleep(1);
    }

    for (int i = 0; i < countServers; i++) {
        Server_delete(servers[i]);
    }

    free(serverEvents);
    return 0;
}
