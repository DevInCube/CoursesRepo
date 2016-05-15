#include <stdio.h>
#include <stdlib.h>

#include "train.h"

void onStart(void * listener,
    train_t * sender,
    start_event_args_t * args) {
    printf("In function %s:\n", __func__);
    printf("\tGood bye!\n");
}

void onStart2(void * listener,
    train_t * sender,
    start_event_args_t * args) {
    printf("In function %s:\n", __func__);
    printf("\tololo\n");
}

typedef struct user_s {
    const char * name;

} user_t;

user_t * user_new(const char * name) {
    user_t * self = malloc(sizeof(user_t));
    self->name = name;
    return self;
}

void user_free(user_t * self) {
    free(self);
}

const char * user_getName(user_t * self) {
    return self->name;
}

void onTrainStarted(
    user_t * listener,
    train_t * sender,
    start_event_args_t * args) {

    printf("In method of user %s:\n", user_getName(listener));
    printf("\tI got notification about train started!\n");
}

int main(void) {
    train_t * train = train_new();
    car_t * cars[] = {
        car_new(CAR_PASSENGER),
        car_new(CAR_FREIGHT),
        car_new(CAR_PASSENGER),
        car_new(CAR_PASSENGER),
        car_new(CAR_FREIGHT),
        car_new(CAR_FREIGHT),
        car_new(CAR_PASSENGER),
    };
    const int carsLength = sizeof(cars) / sizeof(cars[0]);
    for (int i = 0; i < carsLength; i++) {
        train_addCar(train, cars[i]);
    }

    //
    train_subscribeStart(train, NULL, onStart);
    train_subscribeStart(train, NULL, onStart2);
    //
    user_t * user1 = user_new("Ivan");
    user_t * user2 = user_new("Petro");
    train_subscribeStart(train, user1, (start_fn)onTrainStarted);
    train_subscribeStart(train, user2, (start_fn)onTrainStarted);

    // unsubscribe some subscribers
    train_unsubscribeStart(train, NULL, onStart2);
    train_unsubscribeStart(train, user2, (start_fn)onTrainStarted);

    printf("Train cars type:\n");
    list_t * allcars = train_getCarList(train);
    for (int i = 0; i < list_getSize(allcars); i++) {
        car_t * car = (car_t *)list_get(allcars, i);
        car_type_t type = car_getType(car);
        printf("\t%i [%s]\n", i, car_type_toString(type));
    }
    list_free(allcars);

    printf("\nTotal seats: %i\n", train_getAllSeatsCount(train));

    printf("Starting train...\n");
    train_start(train);
    if (train_getState(train) == TRAIN_MOVING) {
        printf("[OK] Train has started!\n");
    } else {
        printf("[ERROR] Train failed to start!\n");
    }

    printf("Stopping train...\n");
    train_stop(train);

    for(int i = 0; i < carsLength; i++) {
        car_free(cars[i]);
    }
    user_free(user1);
    user_free(user2);
    train_free(train);
    return 0;
}
