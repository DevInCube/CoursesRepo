#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <windows.h>
#include "thread.h"
#include "mutex.h"

typedef struct shared_s {
    char sChar;
    mutex_t * mu;
} shared_t;

void * setterFunc(void * args) {
    shared_t * shared = (shared_t *)args;
    while (1) {
        //mutex_lock(shared->mu);
        shared->sChar = rand() % ('Z' - 'A') + 'A';
        //mutex_unlock(shared->mu);
    }
    return NULL;
}

void * writerFunc(void * args) {
    shared_t * shared = (shared_t *)args;
    while (1) {
        //mutex_lock(shared->mu);
        if(shared->sChar == 'O') {
            printf("%c", shared->sChar);
        }
        //mutex_unlock(shared->mu);
        Sleep(1);
    }
    return NULL;
}

int main(void) {
    srand(time(NULL));
    shared_t sharedData;
    sharedData.sChar = 'A';
    sharedData.mu = mutex_new();
    thread_t * setter1 = thread_create_args(setterFunc, &sharedData);
    thread_t * setter2 = thread_create_args(setterFunc, &sharedData);
    thread_t * writer = thread_create_args(writerFunc, &sharedData);
    thread_join(writer);  // wait here
    thread_free(setter1);
    thread_free(setter2);
    thread_free(writer);
    mutex_free(sharedData.mu);
    return 0;
}
