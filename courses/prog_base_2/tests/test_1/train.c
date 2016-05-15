#include "train.h"

const int PASSENGER_CAR_SEATS = 20;

struct car_s {
    car_type_t type;
    int seats;
    int weight;
};

car_t * car_new(car_type_t type) {
    car_t * self = malloc(sizeof(car_t));
    self->type = type;
    self->seats = (CAR_FREIGHT == type) ? 0 : PASSENGER_CAR_SEATS;
    self->weight = (CAR_FREIGHT == type) ? 20 : 5;
    return self;
}

void car_free(car_t * self) {
    free(self);
}

struct train_s {
    train_state_t state;
    list_t * subscribers;
    list_t * cars;
};

typedef struct subscriber_s {
    void * listener;
    start_fn callback;
} subscriber_t;

train_t * train_new() {
    train_t * self = malloc(sizeof(train_t));
    self->state = TRAIN_STOPPED;
    self->subscribers = list_new();
    self->cars = list_new();
    return self;
}

void train_free(train_t * self) {
    list_free(self->subscribers);
    list_free(self->cars);
    free(self);
}

void train_subscribeStart(train_t * self, void * listener, start_fn callback) {
    subscriber_t * sub = malloc(sizeof(subscriber_t));
    sub->listener = listener;
    sub->callback = callback;
    list_push_back(self->subscribers, sub);
}

void train_unsubscribeStart(train_t * self, void * listener, start_fn callback) {
    for (int i = 0; i < list_getSize(self->subscribers); i++) {
        subscriber_t * subs = (subscriber_t *)list_get(self->subscribers, i);
        if ((subs->listener == listener) && (subs->callback == callback)) {
            list_remove(self->subscribers, i);
            break;
        }
    }
}

void train_addCar(train_t * self, car_t * car) {
    list_push_back(self->cars, car);
}

car_t * train_removeCar(train_t * self) {
    car_t * car = (car_t *)list_pop_back(self->cars);
    return car;
}

list_t * train_getCarList(train_t * self) {
    list_t * copy = list_new_copy(self->cars);
    return copy;
}

int __getTotalWeight(train_t * self) {
    int weight = 0;
    for (int i = 0; i < list_getSize(self->cars); i++) {
        car_t * car = list_get(self->cars, i);
        weight += car->weight;
    }
    return weight;
}

void train_start(train_t * self) {
    const int totalWeight = __getTotalWeight(self);
    const int MAX_WEIGHT = 100;
    if (totalWeight <= MAX_WEIGHT) {
        self->state = TRAIN_MOVING;
        start_event_args_t args;
        time(&(args.startTime));
        args.numCars = list_getSize(self->cars);
        args.totalWeight = totalWeight;
        for (int i = 0; i < list_getSize(self->subscribers); i++) {
            subscriber_t * sub = (subscriber_t *)list_get(self->subscribers, i);
            sub->callback(sub->listener, self, &args);
        }
    }
}

void train_stop(train_t * self) {
    self->state = TRAIN_STOPPED;
}

train_state_t train_getState(train_t * self) {
    return self->state;
}

int train_getAllSeatsCount(train_t * self) {
    int seats = 0;
    for (int i = 0; i < list_getSize(self->cars); i++) {
        car_t * car = list_get(self->cars, i);
        seats += car->seats;
    }
    return seats;
}

car_type_t car_getType(car_t * self) {
    return self->type;
}

const char * car_type_toString(car_type_t type) {
    switch (type) {
        case (CAR_PASSENGER) :  return "CAR_PASSENGER";
        case (CAR_FREIGHT) :    return "CAR_FREIGHT";
        default :               return NULL;
    }
}
