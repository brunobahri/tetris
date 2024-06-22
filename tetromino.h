#ifndef TETROMINO_H
#define TETROMINO_H

#define WIDTH 10
#define HEIGHT 20

typedef struct {
    int x, y;
    int shape[4][4];
} Tetromino;

extern Tetromino current;
extern int board[HEIGHT][WIDTH];

void initTetromino();
int canMove(int dx, int dy);
void moveTetromino(int dx, int dy);
void handleInput();

#endif
