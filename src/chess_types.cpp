#include "chess_types.h"


Move::Move(){
    is_promotion = false;
    en_passant = false;
    castling = false;
    capture = false;
    check = false;
    half_move = false;
    pawn_double_push = false;
    color = COLOR_NB;
    piece_type = PIECE_TYPE_NB;
    from = SQUARE_NB;
    to = SQUARE_NB;
    promotion = PIECE_TYPE_NB;
}

Bitboard shift(Bitboard b, int8_t n, int8_t dir){
    if(dir >= 0){
        return b << n;
    } else {
        return b >> n;
    }
}


u_int32_t getBitIndex(Bitboard bitboard){
    if(bitboard == 0)
        return -1;
    int index = 0;
    while(bitboard != 1ULL){
        bitboard = bitboard >> 1;
        index++;
    }
    return index;
}


Bitboard NOT(Bitboard bitboard){
    return ~bitboard;
}


Bitboard randomU64(){
    Bitboard random = 0;
    for(int i = 0; i < 64; i++){
        if(rand() % 2 == 0)
            setBit(random, i);
    }
    return random;
}


void setBit(Bitboard &bitboard, int index){
    bitboard = bitboard | (1ULL << index);
}


int countBits(Bitboard bb){
    int count = 0;
    while(bb){
        count++;
        bb &= bb - 1;
    }
    return count;
}
