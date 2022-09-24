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

    std::string to_string();
};


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
    void make_move(Move move);
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