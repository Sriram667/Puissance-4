#include <ncurses.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

#define SIZE 16
#define HEIGHT 16
#define PLAYERS 2
char CHIP[PLAYERS] = {'X', 'O'};
char board[SIZE][SIZE];

void INIT_SCREEN(void) {
    initscr();
    raw();
    keypad(stdscr, TRUE);
    noecho();
}

void DONE_SCREEN(void) {
    endwin();
    exit(0);
}

void init_board(void) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            board[i][j] = ' ';
        }
    }
}

void draw_board(void) {
    move(0,0);
    for (int i = SIZE - 1; i >= 0; i--) {
        for (int j = 0; j < SIZE; j++) {
            printw("|%c", board[i][j] ? board[i][j] : ' ');
        }
        printw("|\n");
    }
    for (int j = 0; j < SIZE; j++) {
        printw("--");
    }
    printw("-\n");
    for (int j = 0; j < SIZE; j++) {
        printw(" %c", 'A' + j);
    }
    printw("\n");
    refresh();
}

int get_col(void) {
    int ch;
    while (1) {
        ch = getch();
        if (ch == KEY_BACKSPACE || ch == 127) return -1;
        ch = toupper(ch);
        if (ch >= 'A' && ch < 'A' + SIZE) return ch - 'A';
        printw("Entrée invalide, entrez une lettre entre A et P.\n");
        refresh();
    }
}

int add_coin(int col, int player) {
    for (int i = 0; i < SIZE; i++) {
        if (board[i][col] == ' ') {
            board[i][col] = CHIP[player];
            return (player + 1) % PLAYERS;
        }
    }
    return player;
}

int game_over(void) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (board[i][j] == ' ') continue;
            char p = board[i][j];
            if (j <= SIZE - 4 && board[i][j+1] == p && board[i][j+2] == p && board[i][j+3] == p)
                return p;
            if (i <= SIZE - 4 && board[i+1][j] == p && board[i+2][j] == p && board[i+3][j] == p)
                return p;
            if (i <= SIZE - 4 && j <= SIZE - 4 && board[i+1][j+1] == p && board[i+2][j+2] == p && board[i+3][j+3] == p)
                return p;
            if (i >= 3 && j <= SIZE - 4 && board[i-1][j+1] == p && board[i-2][j+2] == p && board[i-3][j+3] == p)
                return p;
        }
    }
    for (int j = 0; j < SIZE; j++) {
        if (board[SIZE - 1][j] == ' ') return 0;
    }
    return 1;
}

void play(void) {
    int player = 0;
    int col;
    int winner = 0;

    while (!winner) {
        draw_board();
        printw("Joueur %d (%c), entrez une colonne (A-P) : ", player + 1, CHIP[player]);
        refresh();

        col = get_col();
        if (col == -1) continue;

        player = add_coin(col, player);
        winner = game_over();
    }

    draw_board();
    if (winner == 1) {
        printw("Match nul !\n");
    } else {
        printw("Joueur %d (%c) gagne !\n", (winner == 'X' ? 1 : 2), winner);
    }

    refresh();
    getch();
}

int main(void) {
    INIT_SCREEN();
    init_board();
    play();
    DONE_SCREEN();
    return 0;
}
