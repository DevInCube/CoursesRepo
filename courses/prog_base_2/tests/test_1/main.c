#include <stdio.h>
#include <stdlib.h>

#include "train.h"

void onStart(void * listener,
    train_t * sender,
    start_event_args_t * args) {

    printf("Good bye!\n");
}

void onStart2(void * listener,
    train_t * sender,
    start_event_args_t * args) {

    printf("ololo\n");
}


typedef struct user_s {
    char * name;

} user_t;

char * user_getName(user_t * self) {
    return self->name;
}

void onTrainStarted(
    user_t * listener,
    train_t * sender,
    start_event_args_t * args) {

    printf("%s got notification about train started", user_getName(listener));
}

int main(void) {

    train_t * train = train_new();
    train_pushVagon(train, vagon_new(PASSAZHIRSKII));
    train_pushVagon(train, vagon_new(PASSAZHIRSKII));
    train_pushVagon(train, vagon_new(VANTAZHNII));
    train_pushVagon(train, vagon_new(PASSAZHIRSKII));
    train_pushVagon(train, vagon_new(VANTAZHNII));

    user_t * user1 = user_new("Ivan");
    user_t * user2 = user_new("Petro");
    train_subscribeStart(train, user1, (start_fn)onTrainStarted);
    train_subscribeStart(train, user2, (start_fn)onTrainStarted);

    train_subscribeStart(train, NULL, onStart);
    train_subscribeStart(train, NULL, onStart2);

    list_t * vagons = train_getVagonList(train);
    for (int i = 0; i < list_getLength(vagons)); i++) {
        vagon_t * vagon = (vagon_t *)list_getAt(vagons, i);
        vagon_type_t type = vagon_getType(vagon);
        if (type == PASSAZHIRSKII) {
            printf("%i > PASSAZHIRSKII vagon\n", i);
        } else {
            printf("%i > VANTAZHNII vagon\n", i);
        }
    }
    list_free(vagons);

    printf("Total seats: %i\n", train_getAllSeats(train));

    train_start(train);
    if (train_getState(train) == RUNNING) {
        printf("hurray!\n");
    } else {
        printf("=(\n");
    }

    train_stop(train);

    train_free(train);
    return 0;
}



/*

Поїзд

Отримати стан поїзда: рухається або стоїть.

Додати вагон до поїзда (у кінець).
Вагон може бути 	пасажирським або вантажним.
Можна 	додавати чи від'єднувати вагони тільки 	коли поїзд стоїть.

Відчепити вагон від поїзда (з кінця)

Отримати кількість та список вагонів

Отримати загальну кількість пасажирських місць

Отримати кількість 	та список вантажних або пасажирських вагонів.

Запустити поїзд (поїзд не може зрушити, якщо має більше 100 тон вантажу).
1 вантажний вагон — 20 тон, 1 пасажирський — 5 тон.

Зупинити поїзд.

Подія: старт поїзда.
Передати у колбек час старту, кількість вагонів та загальну вагу вантажу.

*/
