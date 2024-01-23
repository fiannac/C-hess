#include "moves_generator.h"
#include "magic_bitboards.h"

void MovesGenerator::generateQueenMoves(const Game& game, std::list<Move> &moves){
    Color turn_player = game.turn;
    Color opponent_player = (game.turn == WHITE) ? BLACK : WHITE;
    Bitboard queens = game.pieces[turn_player][QUEEN];
    while(queens){
        Bitboard white_queen = queens & -queens;
        int index = getBitIndex(queens);
            
        Bitboard queen_moves = (
            rookDB[index][magic_hash(game.all & rook_masks[index], rook_magic_numbers[index], rook_db_digits)] |
            bishopDB[index][magic_hash(game.all & bishop_masks[index], bishop_magic_numbers[index], bishop_db_digits)]
        ) & ~game.occupied[turn_player];

        while(queen_moves){
            Move move;
            move.color = turn_player;
            move.piece_type = QUEEN;
            move.to = queen_moves & -queen_moves;
            move.from = white_queen;
            move.capture = game.occupied[opponent_player] & move.to;
            move.en_passant = false;
            move.castling = false;
            move.is_promotion = false;
            if(isLegalMove(game, move)){
                moves.push_back(move);
            }
            queen_moves &= queen_moves - 1;
        }
        queens &= queens - 1;
    }
}
