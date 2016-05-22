#pragma once

/**
*   Objects of this type can call back functions on file system directory changes in files
*/
typedef struct fswatcher_s fswatcher_t;

typedef enum fschange_type_s {
    FILE_ADDED,
    FILE_REMOVED,
    FILE_MODIFIED,
    FILE_RENAMED_OLD,
    FILE_RENAMED_NEW,
} fschange_type_t;

/**
*   a callback functions type to react on file system changes
*/
typedef void (*fschange_fn)(fschange_type_t type, const char * filename, const char * directory, void * context);

/**
*   @return new FileSystemWatcher that starts working in new thread or NULL on fail
*/
fswatcher_t * fswatcher_new(const char * dirPath);

/**
*   subscribe listeners on file changes notifications
*/
void fswatcher_subscribe(fswatcher_t *, fschange_fn callback, void * context);

/**
*   unsubscribe listeners from file changes notifications
*/
void fswatcher_unsubscribe(fswatcher_t * self, fschange_fn callback, void * context);

/**
*   start new thread to listen for file events there
*/
void fswatcher_start(fswatcher_t * self);

/**
*   start the listening thread
*/
void fswatcher_stop(fswatcher_t * self);

/**
*   stops FileSystemWatcher thread and frees resources
*/
void fswatcher_free(fswatcher_t * self);
