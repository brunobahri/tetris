#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <termios.h>
#include <sys/time.h>
#include "tetromino.h"
#include "board.h"
#include "input.h"

int delay = 500000; 
int dropDelay = 500000; 
int fastDrop = 0; 

void setTerminalMode(struct termios *oldt, struct termios *newt) {
    tcgetattr(STDIN_FILENO, oldt);
    *newt = *oldt;
    newt->c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, newt);
}

void resetTerminalMode(struct termios *oldt) {
    tcsetattr(STDIN_FILENO, TCSANOW, oldt);
}

int main() {
    struct termios oldt, newt;
    setTerminalMode(&oldt, &newt);

    initTetromino();

    struct timespec lastTime, currentTime;
    clock_gettime(CLOCK_MONOTONIC, &lastTime);

    while (1) {
        handleInput();

        clock_gettime(CLOCK_MONOTONIC, &currentTime);
        long elapsed = (currentTime.tv_sec - lastTime.tv_sec) * 1000000L + 
                       (currentTime.tv_nsec - lastTime.tv_nsec) / 1000L;

        if (fastDrop) {
            dropDelay = 50000; // Acelera a queda
        } else {
            dropDelay = delay;
        }

        if (elapsed >= dropDelay) {
            if (canMove(0, 1)) {
                moveTetromino(0, 1);
            } else {
                for (int i = 0; i < 4; i++) {
                    for (int j = 0; j < 4; j++) {
                        if (current.shape[i][j]) {
                            board[current.y + i][current.x + j] = current.shape[i][j];
                        }
                    }
                }
                clearFullLines();
                initTetromino();
            }
            lastTime = currentTime;
        }

        drawBoard();
        usleep(100000); 
    }

    resetTerminalMode(&oldt);
    return 0;
}
