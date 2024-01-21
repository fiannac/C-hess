#ifndef CHESS_TYPES_H
#define CHESS_TYPES_H

#include <cstdint>
#include <string>
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

enum Square {
    H1 = 0, G1, F1, E1, D1, C1, B1, A1,
    H2, G2, F2, E2, D2, C2, B2, A2,
    H3, G3, F3, E3, D3, C3, B3, A3,
    H4, G4, F4, E4, D4, C4, B4, A4,
    H5, G5, F5, E5, D5, C5, B5, A5,
    H6, G6, F6, E6, D6, C6, B6, A6,
    H7, G7, F7, E7, D7, C7, B7, A7,
    H8, G8, F8, E8, D8, C8, B8, A8,
    SQUARE_NB
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
    bool check;
    bool half_move;
    bool illegal_move;
    bool pawn_double_push;


    Move() {
        is_promotion = false;
        en_passant = false;
        castling = false;
        capture = false;
        check = false;
        half_move = false;
        illegal_move = false;
        pawn_double_push = false;
    }

    std::string to_string();
};

const Bitboard FILE_A = 0x8080808080808080;
const Bitboard FILE_B = 0x4040404040404040;
const Bitboard FILE_C = 0x2020202020202020;
const Bitboard FILE_D = 0x1010101010101010;
const Bitboard FILE_E = 0x0808080808080808;
const Bitboard FILE_F = 0x0404040404040404;
const Bitboard FILE_G = 0x0202020202020202;
const Bitboard FILE_H = 0x0101010101010101;

const Bitboard RANK_1 = 0x00000000000000FF;
const Bitboard RANK_2 = 0x000000000000FF00;
const Bitboard RANK_3 = 0x0000000000FF0000;
const Bitboard RANK_4 = 0x00000000FF000000;
const Bitboard RANK_5 = 0x000000FF00000000;
const Bitboard RANK_6 = 0x0000FF0000000000;
const Bitboard RANK_7 = 0x00FF000000000000;
const Bitboard RANK_8 = 0xFF00000000000000;


#endif
