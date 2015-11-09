#include <stdlib.h>
#include <stdio.h>

#include "balls.h"
#include "console.h"

void init(Ball *ball) {
    const int MAX_VEL = 50;
    const int MIN_VEL = 2;
    ball->Pos.X = rand() % SCREEN_WIDTH;
    ball->Pos.Y = rand() % SCREEN_HEIGHT;
    ball->Vel.X = rand() % MAX_VEL - MAX_VEL / 2;
    if(abs(ball->Vel.X) < MIN_VEL)
        ball->Vel.X = MIN_VEL;
    ball->Vel.Y = rand() % MAX_VEL - MAX_VEL / 2;
    if(abs(ball->Vel.Y) < MIN_VEL)
        ball->Vel.Y = MIN_VEL;
    ball->Color = (rand() % 7) + 1;
}

void update(Ball *ball) {
    ball->Pos.X += ball->Vel.X;
    ball->Pos.Y += ball->Vel.Y;
    if(ball->Pos.X >= SCREEN_WIDTH || ball->Pos.X <= 0) {
        ball->Vel.X = -ball->Vel.X;
        ball->Pos.X += ball->Vel.X;
    }
    if(ball->Pos.Y >= SCREEN_HEIGHT || ball->Pos.Y <= 0) {
        ball->Vel.Y = -ball->Vel.Y;
        ball->Pos.Y += ball->Vel.Y;
    }
}

void draw(Ball *b) {
    /* from floating screen to console coordinates */
    int cx = b->Pos.X / SCREEN_WIDTH * CONSOLE_COLUMNS;
    int cy = b->Pos.Y / SCREEN_HEIGHT * CONSOLE_ROWS;
    setTextColor(b->Color);
    moveCursor(cx, cy);
    printf("O");
    clearFormat();
}

void initBalls(int count_balls, Ball * ballsList) {
    for(int i = 0; i < count_balls; i++) {
        init(&ballsList[i]);
    }
}

void updateBalls(int count_balls, Ball * ballsList) {
    for(int i = 0; i < count_balls; i++) {
        update(&ballsList[i]);
    }
}

void drawBalls(int count_balls, Ball * ballsList) {
    for(int i = 0; i < count_balls; i++) {
        draw(&ballsList[i]);
    }
}

void print(Ball *b) {
    printf("Position: %f %f\n", b->Pos.X, b->Pos.Y);
    printf("Velocity: %f %f\n", b->Vel.X, b->Vel.Y);
}
