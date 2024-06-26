#include <sys/select.h>
#include <unistd.h>
#include "input.h"
#include "tetromino.h"

extern int fastDrop; // Declaração externa para acessar a variável global

int kbhit() {
    struct timeval tv = { 0L, 0L };
    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(STDIN_FILENO, &fds);
    return select(STDIN_FILENO + 1, &fds, NULL, NULL, &tv);
}

void handleInput() {
    if (kbhit()) {
        char input;
        read(STDIN_FILENO, &input, 1);
        switch (input) {
            case 'a':
                moveTetromino(-1, 0);
                break;
            case 'd':
                moveTetromino(1, 0);
                break;
            case 's':
                fastDrop = 1; 
                break;
            case 'q':
                fastDrop = 0; 
                break;
        }
    } else {
        fastDrop = 0;
    }
}
