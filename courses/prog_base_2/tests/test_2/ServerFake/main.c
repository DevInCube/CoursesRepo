#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "server_fake.h"

int main(void) {
    server_t * server = server_new();
    char response[1024] = "";
    while (1) {
        const char * request = server_read(server);
        printf("Server> %s\n", request);
        // create response
        strcpy(response, "RESPONSE!");
        server_write(server, response);
    }
    server_free(server);
    return 0;
}
