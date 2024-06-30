#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <sys/time.h>
#include <string.h>
#include "tetromino.h"
#include "board.h"
#include "input.h"

#define BUFFER_SIZE 1024*1024

int delay = 500000; // 0.5 segundos
int dropDelay = 500000; // Inicialmente, o delay é o mesmo
int fastDrop = 0; // Flag para controlar a descida rápida
int score = 0; // Variável de pontuação

void setTerminalMode(struct termios *oldt, struct termios *newt) {
    tcgetattr(STDIN_FILENO, oldt);
    *newt = *oldt;
    newt->c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, newt);
}

void resetTerminalMode(struct termios *oldt) {
    tcsetattr(STDIN_FILENO, TCSANOW, oldt);
}

void drawBoardWithScore() {
    char buffer[BUFFER_SIZE];
    int offset = 0;

    // Obtendo o tamanho do terminal
    struct winsize w;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

    int terminalWidth = w.ws_col;
    int terminalHeight = w.ws_row;

    // Calculando a posição centralizada
    int startX = (terminalWidth - WIDTH - 2) / 2; // -2 para incluir as bordas laterais
    int startY = (terminalHeight - HEIGHT - 3) / 2; // -3 para incluir a linha do score e as bordas superior e inferior

    // Desenhando o score no topo
    offset += snprintf(buffer + offset, BUFFER_SIZE - offset, "\033[%d;%dHScore: %d", startY, startX + 2, score);

    // Desenhando a borda superior
    offset += snprintf(buffer + offset, BUFFER_SIZE - offset, "\033[%d;%dH+", startY + 1, startX);
    for (int i = 0; i < WIDTH; i++) {
        offset += snprintf(buffer + offset, BUFFER_SIZE - offset, "-");
    }
    offset += snprintf(buffer + offset, BUFFER_SIZE - offset, "+");

    // Desenhando o tabuleiro com a peça em movimento e as bordas laterais
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
        offset += snprintf(buffer + offset, BUFFER_SIZE - offset, "\033[%d;%dH|", startY + 2 + i, startX);
        for (int j = 0; j < WIDTH; j++) {
            if (display[i][j]) {
                offset += snprintf(buffer + offset, BUFFER_SIZE - offset, "#");
            } else {
                offset += snprintf(buffer + offset, BUFFER_SIZE - offset, ".");
            }
        }
        offset += snprintf(buffer + offset, BUFFER_SIZE - offset, "|");
    }

    // Desenhando a borda inferior
    offset += snprintf(buffer + offset, BUFFER_SIZE - offset, "\033[%d;%dH+", startY + 2 + HEIGHT, startX);
    for (int i = 0; i < WIDTH; i++) {
        offset += snprintf(buffer + offset, BUFFER_SIZE - offset, "-");
    }
    offset += snprintf(buffer + offset, BUFFER_SIZE - offset, "+");

    // Escrevendo o buffer inteiro na tela de uma vez
    write(STDOUT_FILENO, buffer, offset);
}

int main() {
    struct termios oldt, newt;
    setTerminalMode(&oldt, &newt);

    srand(time(NULL)); // Configurar a semente aleatória

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
                // Adicionar a peça ao tabuleiro e iniciar uma nova peça
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

        drawBoardWithScore();
        usleep(100000); // Pequeno atraso para limitar o loop
    }

    resetTerminalMode(&oldt);
    return 0;
}
