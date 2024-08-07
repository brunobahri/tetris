#ifndef TETROMINO_H
#define TETROMINO_H
#define WIDTH 16
#define HEIGHT 20

typedef struct {
    int x, y;
    int shape[4][4];
} Tetromino;

extern Tetromino current;
extern int board[HEIGHT][WIDTH];
extern int delay;
extern int dropDelay;
extern int fastDrop;
extern int score;

void initTetromino();
int canMove(int dx, int dy);
void moveTetromino(int dx, int dy);
void rotateTetromino();
void handleInput();
void dropTetromino();
void clearFullLines();

#endif
