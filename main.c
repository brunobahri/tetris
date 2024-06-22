#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <termios.h>
#include "tetromino.h"
#include "board.h"

int main() {
    initTetromino();
    struct termios oldt, newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    while (1) {
        handleInput();
        drawBoard();
        if (canMove(0, 1)) {
            moveTetromino(0, 1);
        } else {
            // Adicionar a peça ao tabuleiro e iniciar uma nova peça
            for (int i = 0; i < 4; i++) {
                for (int j = 0; j < 4; j++) {
                    if (current.shape[i][j]) {
                        board[current.y + i][current.x + j] = current.shape[i][j];
                    }
                }
            }
            initTetromino();
        }
        usleep(500000); // Atraso para simular movimento (0.5 segundos)
    }

    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    return 0;
}
