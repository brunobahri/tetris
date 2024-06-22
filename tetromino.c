#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include "tetromino.h"

Tetromino tetrominos[] = {
    {0, 0, {{1, 1, 1, 1}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}}, // I
    {0, 0, {{1, 1, 1, 0}, {0, 1, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}}, // T
    {0, 0, {{1, 1, 0, 0}, {1, 1, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}}, // O
    // Adicione as outras peças (L, J, S, Z)
};

Tetromino current;
int board[HEIGHT][WIDTH] = {0};

void initTetromino() {
    srand(time(NULL));
    current = tetrominos[rand() % 3]; // Temporariamente usando apenas 3 peças
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

void handleInput() {
    char input;
    if (read(STDIN_FILENO, &input, 1) != 1) return;
    switch (input) {
        case 'a':
            moveTetromino(-1, 0);
            break;
        case 'd':
            moveTetromino(1, 0);
            break;
    }
}
