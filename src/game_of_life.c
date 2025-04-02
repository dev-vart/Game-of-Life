#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define ROWS 25
#define COLS 80
#define LIVE_CELL "*"
#define DEAD_CELL " "

int **create_field();
void output(int **matrix);
void draw(int **matrix, int delay);
void game(int **matrix);
void free_memory(int **matrix);

int main() {
    initscr();
    noecho();
    curs_set(FALSE);
    int freopennull = 0;

    int **matrix = create_field();
    if (freopen("/dev/tty", "r", stdin) == NULL) {
        freopennull = 1;
    } else {
        int delay = 240000;
        draw(matrix, delay);
        free_memory(matrix);

        endwin();
    }
    return freopennull;
}

int **create_field() {
    int **matrix = (int **)malloc(ROWS * sizeof(int *));
    for (int i = 0; i < ROWS; i++) {
        matrix[i] = (int *)malloc(COLS * sizeof(int));
    }
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            scanf("%d", &matrix[i][j]);
        }
    }
    return matrix;
}

void output(int **matrix) {
    clear();
    for (int i = 0; i < ROWS; i++) {
        for (int j = 0; j < COLS; j++) {
            if (matrix[i][j] == 1) {
                printw(LIVE_CELL);
            } else {
                printw(DEAD_CELL);
            }
        }
        printw("\n");
    }
    refresh();
}

void draw(int **matrix, int delay) {
    int end_game = 0;
    while (!end_game) {
        output(matrix);
        game(matrix);
        usleep(delay);
        timeout(0);
        int ch = getch();
        switch (ch) {
            case '-':
                delay += 30000;
                if (delay > 240000) {
                    delay = 240000;
                }
                break;
            case '+':
                delay -= 30000;
                if (delay < 30000) {
                    delay = 30000;
                }
                break;
            case 'q':
                end_game = 1;
                break;
        }
    }
}

void game(int **matrix) {
    int **new_matrix = (int **)malloc(ROWS * sizeof(int *));
    for (int i = 0; i < ROWS; i++) {
        new_matrix[i] = (int *)malloc(COLS * sizeof(int));
    }

    for (int h = 0; h < ROWS; h++) {
        for (int l = 0; l < COLS; l++) {
            int prev_h = h - 1;
            int prev_l = l - 1;
            int next_l = l + 1;
            int next_h = h + 1;

            if (h == 0) prev_h = ROWS - 1;
            if (l == 0) prev_l = COLS - 1;
            if (h == ROWS - 1) next_h = 0;
            if (l == COLS - 1) next_l = 0;

            int n = 0;
            n += matrix[prev_h][prev_l];
            n += matrix[prev_h][l];
            n += matrix[prev_h][next_l];
            n += matrix[h][next_l];
            n += matrix[h][prev_l];
            n += matrix[next_h][prev_l];
            n += matrix[next_h][l];
            n += matrix[next_h][next_l];

            if (matrix[h][l] == 0 && n == 3) {
                new_matrix[h][l] = 1;
            } else if (matrix[h][l] == 1 && (n == 2 || n == 3)) {
                new_matrix[h][l] = 1;
            } else {
                new_matrix[h][l] = 0;
            }
        }
    }
    for (int h = 0; h < ROWS; h++) {
        for (int l = 0; l < COLS; l++) {
            matrix[h][l] = new_matrix[h][l];
        }
    }

    free_memory(new_matrix);
}

void free_memory(int **matrix) {
    for (int i = 0; i < ROWS; i++) {
        free(matrix[i]);
    }
    free(matrix);
}
