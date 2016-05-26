#ifndef SERVER_FAKE_H_INCLUDED
#define SERVER_FAKE_H_INCLUDED

typedef struct server_s server_t;

server_t * server_new(void);
void server_free(server_t * self);
const char * server_read(server_t * self);
void server_write(server_t * self, const char * response);

#endif // SERVER_FAKE_H_INCLUDED
