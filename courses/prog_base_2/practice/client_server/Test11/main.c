#include <stdio.h>
#include <stdlib.h>

int main() {

    post_t * p = post_new(3);
    // 70
    car_t * c = post_getCar(p, 80);
    terminal_t * t = terminal_new(false);
    terminal_processCar(t, c);
    assert_int_equal(terminal_countRecords(t), 0);

    return 0;
}

