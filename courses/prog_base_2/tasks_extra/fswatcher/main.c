#include <stdio.h>
#include <stdlib.h>

#include <windows.h>
#include <conio.h>

#include "fswatcher.h"

static void onFileChanged(fschange_type_t type, const char * filename, const char * directory, void * context) {
    switch (type) {
    case FILE_ADDED:
        printf("<%s>\n", (const char *)context);
        printf("File [%s] created in directory [%s]\n\n", filename, directory);
        break;
    // @todo another types
    default:
        break;
    }
}

int main(void) {
    const char * dir = "C:\\Users\\DevinCube\\Desktop\\dir";
    fswatcher_t * watcher = fswatcher_new(dir);
    if (NULL == watcher) {
        printf("Error with [%s]\n", dir);
    } else {
        fswatcher_subscribe(watcher, &onFileChanged, "Name-1");
        fswatcher_subscribe(watcher, &onFileChanged, "Name-2");
        printf("Start listening to directory [%s] file changes\n\n", dir);
        fswatcher_start(watcher);
    }
    const char * dir2 = "C:\\Users\\DevinCube\\Desktop\\dir2";
    fswatcher_t * watcher2 = fswatcher_new(dir2);
    if (NULL == watcher2) {
        printf("Error with [%s]\n", dir2);
    } else {
        fswatcher_subscribe(watcher2, &onFileChanged, "Name-1");
        fswatcher_subscribe(watcher2, &onFileChanged, "Name-3");
        printf("Start listening to directory [%s] file changes\n\n", dir2);
        fswatcher_start(watcher2);
    }
    getch();  // wait in main thread
    if (NULL != watcher) {
        fswatcher_stop(watcher);
        fswatcher_free(watcher);
    }
    if (NULL != watcher2) {
        fswatcher_stop(watcher2);
        fswatcher_free(watcher2);
    }
    return 0;
}
