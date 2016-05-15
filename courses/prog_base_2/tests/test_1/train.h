#ifndef TRAIN_H_INCLUDED
#define TRAIN_H_INCLUDED

#include <stdlib.h>
#include <time.h>
#include "list/list.h"

typedef struct train_s train_t;
typedef struct car_s car_t;

typedef enum {
    TRAIN_STOPPED,
    TRAIN_MOVING
} train_state_t;

typedef enum {
    CAR_PASSENGER,
    CAR_FREIGHT,
} car_type_t;

train_t * train_new();
void train_free(train_t * self);

train_state_t train_getState(train_t * self);

void train_addCar(train_t * train, car_t * car);
car_t * train_removeCar(train_t * self);

// free list after usage
list_t * train_getCarList(train_t * self);
// free list after usage
list_t * train_getCarListOfType(train_t * self, car_type_t type);

int train_getAllSeatsCount(train_t * self);
void train_start(train_t * self);
void train_stop(train_t * self);

typedef struct start_event_args_s {
    time_t startTime;
    size_t numCars;
    int totalWeight;
} start_event_args_t;

typedef void (*start_fn)(
    void * listener,
    train_t * sender,
    start_event_args_t * args);

void train_subscribeStart(train_t * self, void * listener, start_fn callback);
void train_unsubscribeStart(train_t * self, void * listener, start_fn callback);

car_t * car_new(car_type_t type);
void car_free(car_t * self);

car_type_t car_getType(car_t * self);
const char * car_type_toString(car_type_t type);

#endif // TRAIN_H_INCLUDED
