#ifndef CHESS_TYPES_H
#define CHESS_TYPES_H

#include <cstdint>
#include <string>
#include "squares.h"

typedef std::uint64_t Bitboard;


enum PieceType {
    PAWN = 0,
    KNIGHT,
    BISHOP,
    ROOK,
    QUEEN,
    KING,
    PIECE_TYPE_NB
};

enum Color {
    WHITE = 0,
    BLACK,
    COLOR_NB
};

struct Move {
    Color color;
    PieceType piece_type;
    Bitboard from;
    Bitboard to;
    PieceType promotion;
    bool is_promotion;
    bool en_passant;
    bool castling;
    bool capture;
    bool check;  // Unused.
    bool half_move;
    bool pawn_double_push;

    Move();
    std::string to_string();
};

Bitboard shift(Bitboard b, int8_t n, int8_t dir);
uint32_t getBitIndex(Bitboard);
Bitboard NOT(Bitboard bitboard);
Bitboard randomU64();
void setBit(Bitboard &bitboard, int index);
int countBits(Bitboard);


#endif
