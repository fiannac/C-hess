#ifndef GAME_H
#define GAME_H

#include "chess_types.h"

struct Game {
    Bitboard pieces[COLOR_NB][PIECE_TYPE_NB];
    Bitboard occupied[COLOR_NB];
    Bitboard all;
    Color turn;
    Bitboard en_passant;
    bool en_passant_possible;
    int halfmove_clock;
    int fullmove_number;
    bool white_can_castle_kingside;
    bool white_can_castle_queenside;
    bool black_can_castle_kingside;
    bool black_can_castle_queenside;

    Game(std::string s);
    void make_move(const Move& move);
    std::string to_string();
};


# endif