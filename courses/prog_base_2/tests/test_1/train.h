#ifndef TRAIN_H_INCLUDED
#define TRAIN_H_INCLUDED

typedef struct list_s list_t;

typedef struct train_s train_t;
typedef struct vagon_s vagon_t;

typedef enum {
    STOPPED,
    RUNNING
} train_state_t;

typedef enum {
    PASSAZHIRSKII,
    VANTAZHNII,
} vagon_type_t;

train_t * train_new();
void train_free(train_t * self);

train_state_t train_getState(train_t * self);

void train_pushVagon(train_t * train, vagon_t * vagon);
vagon_t * train_popVagon(train_t * self);

// free list
list_t * train_getVagonList(train_t * self);
// free list
list_t * train_getVagonListOfType(train_t * self, vagon_type_t type);

int train_getAllSeats(train_t * self);
void train_start(train_t * self);
void train_stop(train_t * self);

typedef struct start_event_args_s {
    time_t startTime;
    size_t numVagons;
    int totalWeight;
} start_event_args_t;

typedef void (*start_fn)(
    void * listener,
    train_t * sender,
    start_event_args_t * args);

void train_subscribeStart(train_t * self, void * listener, start_fn callback);
void train_unsubscribeStart(train_t * self, void * listener);

vagon_t * vagon_new(vagon_type_t type);
void vagon_free(vagon_t * self);

vagon_type_t vagon_getType(vagon_t * self);

#endif // TRAIN_H_INCLUDED
