#ifndef UCI_H
#define UCI_H

enum uci_commands {
    uci,
    isready,
    position_startpos,
    position_startpos_moves,
    position_fen,
    go,
    stop,
    quit,
    uci_perft,
    bk,
    test,
};

int parse_line(enum uci_commands uci_in, char* data);

#endif
