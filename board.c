#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "board.h"
#include "tetromino.h"

void drawBoard() {
    system("clear"); 
    int display[HEIGHT][WIDTH];
    memcpy(display, board, sizeof(board));

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (current.shape[i][j]) {
                display[current.y + i][current.x + j] = current.shape[i][j];
            }
        }
    }

    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (display[i][j])
                printf("#");
            else
                printf(".");
        }
        printf("\n");
    }
}
