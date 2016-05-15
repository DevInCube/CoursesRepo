#include "train.h"

struct train_s {
    train_state_t state;
    list_t * subscribers;
    list_t * vagons;
};

typedef struct subscriber_s {
    void * listener;
    start_fn callback;
} subscriber_t;

void train_subscribeStart(train_t * self, void * listener, start_fn callback) {
    subscriber_t * sub = malloc(sizeof(subscriber_t));
    sub->listener = listener;
    sub->callback = callback;
    list_addLast(subscribers, sub);
}

void train_start(train_t * self) {
    if (/**/) {
        self->state = RUNNING;
        start_event_args_t args;
        // args.startTime;
        args.numVagons = 5;
        args.totalWeight = 34;
        for (int i = 0; i < list_getLength(self->subscribers); i++) {
            subscriber_t * sub = (subscriber_t *)list_getAt(self->subscribers, i);
            sub->callback(sub->listener, self, &args);
        }
    }
}

/*
struct vagon_s {
    int type;
    int weight;
    int seats;
};*/
