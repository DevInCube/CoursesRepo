#ifndef FAKSERVER_H_INCLUDED
#define FAKSERVER_H_INCLUDED

// helper macros
#define moduleType(MODULE_NAME) \
    typedef struct MODULE_NAME * MODULE_NAME

#define moduleType_alloc(MNAME) \
    (MNAME)malloc(sizeof(struct MNAME))

// module types
moduleType(Server_T);
moduleType(Server_Events_T);

// special callback type
typedef void (*Server_EventHandler_T)(Server_T sender, void * eventArgs);

// this type has pyblic fields
struct Server_Events_T {
    Server_EventHandler_T    start;
    Server_EventHandler_T    stop;
    Server_EventHandler_T    message;
};

// API functions
Server_T Server_new(const char * serverName, Server_Events_T events, void * userData);
void Server_delete(Server_T s);
const char * Server_getServerName(Server_T server);
void * Server_getUserData(Server_T server);
void Server_start(Server_T s);
void Server_stop(Server_T s);

#endif // FAKSERVER_H_INCLUDED
