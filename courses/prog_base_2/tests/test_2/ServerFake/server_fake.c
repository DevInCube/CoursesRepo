#include <stdlib.h>
#include <stdio.h>

#include "server_fake.h"

struct server_s {
    char * request;
};

static char * getline(void);

server_t * server_new(void) {
    server_t * self = malloc(sizeof(server_t));
    self->request = NULL;
    return self;
}

void server_free(server_t * self) {
    if (NULL != self->request) {
        free(self->request);
    }
    free(self);
}

const char * server_read(server_t * self) {
    printf("Browser input>");
    if (NULL != self->request) {
        free(self->request);
    }
    self->request = getline();
    return self->request;
}

void server_write(server_t * self, const char * response) {
    printf("Browser output> %s\n", response);
}

//

char * getline(void) {
    char * line = malloc(100), * linep = line;
    size_t lenmax = 100, len = lenmax;
    int c;

    if(line == NULL)
        return NULL;

    for(;;) {
        c = fgetc(stdin);
        if(c == EOF)
            break;

        if(--len == 0) {
            len = lenmax;
            char * linen = realloc(linep, lenmax *= 2);

            if(linen == NULL) {
                free(linep);
                return NULL;
            }
            line = linen + (line - linep);
            linep = linen;
        }

        if((*line++ = c) == '\n')
            break;
    }
    *line = '\0';
    return linep;
}
