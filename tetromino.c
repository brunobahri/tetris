#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include "tetromino.h"
#include "input.h"

Tetromino tetrominos[] = {
    {0, 0, {{1, 1, 1, 1}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}}, // I
    {0, 0, {{1, 1, 1, 0}, {0, 1, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}}, // T
    {0, 0, {{1, 1, 0, 0}, {1, 1, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}}, // O
    {0, 0, {{1, 1, 0, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}}, // Z
    {0, 0, {{0, 1, 1, 0}, {1, 1, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}}, // S
    {0, 0, {{1, 1, 1, 0}, {1, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}}, // L
    {0, 0, {{1, 1, 1, 0}, {0, 0, 1, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}}, // J
};

Tetromino current;
int board[HEIGHT][WIDTH] = {0};
extern int dropDelay;
extern int delay;
extern int fastDrop;
extern int score;

void initTetromino() {
    current = tetrominos[rand() % 7]; // Usando todas as 7 peças
    current.x = WIDTH / 2 - 2;
    current.y = 0;
}

int canMove(int dx, int dy) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (current.shape[i][j]) {
                int newX = current.x + j + dx;
                int newY = current.y + i + dy;
                if (newX < 0 || newX >= WIDTH || newY < 0 || newY >= HEIGHT || board[newY][newX]) {
                    return 0;
                }
            }
        }
    }
    return 1;
}

void moveTetromino(int dx, int dy) {
    if (canMove(dx, dy)) {
        current.x += dx;
        current.y += dy;
    }
}

void rotateTetromino() {
    Tetromino temp = current;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            temp.shape[j][3 - i] = current.shape[i][j];
        }
    }
    if (canMove(0, 0)) {
        current = temp;
    }
}

void dropTetromino() {
    while (canMove(0, 1)) {
        moveTetromino(0, 1);
    }
}

void clearFullLines() {
    for (int i = 0; i < HEIGHT; i++) {
        int full = 1;
        for (int j = 0; j < WIDTH; j++) {
            if (!board[i][j]) {
                full = 0;
                break;
            }
        }
        if (full) {
            score += 10;
            for (int k = i; k > 0; k--) {
                for (int j = 0; j < WIDTH; j++) {
                    board[k][j] = board[k - 1][j];
                }
            }
            for (int j = 0; j < WIDTH; j++) {
                board[0][j] = 0;
            }
        }
    }
}
