#include <stdio.h>
#include <stdlib.h>

#include "socket.h"
#include "cJSON.h"

int main(void) {
    socket_t * sock = socket_new();
    socket_free(sock);
    return 0;
}
