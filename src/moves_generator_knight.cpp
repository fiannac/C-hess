#include "moves_generator.h"


void MovesGenerator::inizializeKnightMasks(){
    Bitboard index = 1ULL;
    for(int i=0;i<64;i++){
        knight_masks[i] = 
            (index & ~FILE_H) >> 17 |
            (index & ~(FILE_G | FILE_H)) >> 10 |
            (index & ~(FILE_G | FILE_H)) << 6 |
            (index & ~FILE_H) << 15 | 
            (index & ~FILE_A) >> 15 |
            (index & ~(FILE_A | FILE_B)) >> 6 | 
            (index & ~(FILE_A | FILE_B)) << 10 |
            (index & ~FILE_A) << 17;
        index <<= 1;
    }
}

void MovesGenerator::generateKnightMoves(const Game& game, std::list<Move> &moves){
    Color turn_player = game.turn;
    Color opponent_player = (game.turn == WHITE) ? BLACK : WHITE;
    Bitboard knights = game.pieces[turn_player][KNIGHT];

    while(knights){

        Bitboard knight = knights & -knights;
        Bitboard knight_moves = knight_masks[getBitIndex(knight)] & ~game.occupied[turn_player];

        while(knight_moves){
            Move move;
            move.color = turn_player;
            move.piece_type = KNIGHT;
            move.to = knight_moves & -knight_moves;
            move.from = knight;
            move.capture = game.occupied[opponent_player] & move.to;
            move.en_passant = false;
            move.castling = false;
            move.is_promotion = false;
            move.check = false;

            if(isLegalMove(game, move)){
                moves.push_back(move);
            }

            knight_moves &= knight_moves - 1;
        }

        knights &= knights - 1;
    }
}
