#include "chess_types.h"


Move::Move(){
    is_promotion = false;
    en_passant = false;
    castling = false;
    capture = false;
    check = false;
    half_move = false;
    illegal_move = false;
    pawn_double_push = false;
}


Move::Move(
    Color color,
    PieceType piece_type,
    Bitboard from,
    Bitboard to,
    PieceType promotion,
    bool is_promotion,
    bool en_passant,
    bool castling,
    bool capture,
    bool check,
    bool half_move,
    bool illegal_move,
    bool pawn_double_push
){
    this->color = color;
    this->piece_type = piece_type;
    this->from = from;
    this->to = to;
    this->promotion = promotion;
    this->is_promotion = is_promotion;
    this->en_passant = en_passant;
    this->castling = castling;
    this->capture = capture;
    this->half_move = half_move;
    this->illegal_move = illegal_move;
    this->pawn_double_push = pawn_double_push;
}


Bitboard shift(Bitboard b, int8_t n, int8_t dir){
    if(dir >= 0){
        return b << n;
    } else {
        return b >> n;
    }
}
