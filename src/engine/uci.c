#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bk.h"
#include "board.h"
#include "move.h"
#include "perft.h"
#include "search.h"
#include "uci.h"
#include "util.h"

#include <fxcg/system.h>

static Board board;
static Search search;

void handle_uci() {
    printf("id name Mister Queen\n");
    printf("id author Michael Fogleman\n");
    printf("uciok\n");
}

void handle_isready() {
    printf("readyok\n");
}

void handle_fen(char *fen) {
    board_load_fen(&board, fen);
}

void handle_startpos() {
    board_reset(&board);
}

void handle_startpos_moves(char *moves) {
    Move move;
    board_reset(&board);
    char *key;
    char *token = tokenize(moves, " ", &key);
    while (token) {
        move_from_string(&move, token);
        make_move(&board, &move);
        token = tokenize(NULL, " ", &key);
    }
}

static int thread_func(void *arg) {
    do_search(&search, &board);
    return 0;
}

static void thread_start() {
    Timer_Install(&thrd, thread_func, NULL);
}

void handle_go(char *line) {
    search.uci = 1;
    search.use_book = 1;
    search.duration = 4;
    char *key;
    char *token = tokenize(line, " ", &key);
    while (token) {
        if (strcmp(token, "infinite") == 0) {
            search.duration = 0;
            search.use_book = 0;
        }
        else if (strcmp(token, "movetime") == 0) {
            char *arg = tokenize(NULL, " ", &key);
            search.duration = atoi(arg) / 1000.0;
        }
        else if (strcmp(token, "ponder") == 0) {
            return; // no pondering yet
        }
        token = tokenize(NULL, " ", &key);
    }
    thread_start();
}

void handle_stop() {
    search.stop = 1;
    thrd_join(thrd, NULL);
}

int parse_line(enum uci_commands uci_in, char* data) {
    if (uci_in == uci) {
        handle_uci();
    }
    if (uci_in == isready) {
        handle_isready();
    }
    if (uci_in == position_startpos) {
        handle_startpos();
    }
    if (uci_in == position_startpos_moves) {
        handle_startpos_moves(data);
    }
    if (uci_in == position_fen) {
        handle_fen(data);
    }
    if (uci_in == go) {
        handle_go(data);
    }
    if (uci_in == stop) {
        handle_stop();
    }
    if (uci_in == quit) {
        return 0;
    }
    if (uci_in == uci_perft) {
        perft_tests();
    }
    if (uci_in == bk) {
        bk_tests();
    }
    if (uci_in == test) {
        test_position(atoi(data));
    }
    return 1;
}

/*
void uci_main() {
    setbuf(stdout, NULL);
    board_reset(&board);
    while (parse_line());
}
*/
