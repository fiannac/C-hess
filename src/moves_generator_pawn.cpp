#include "moves_generator.h"
#include "chess_types.h"


void MovesGenerator::generatePawnMoves(const Game& game, std::list<Move> &moves){
    Color turn_player = game.turn;
    Color opponent_player = (game.turn == WHITE) ? BLACK : WHITE;
    int8_t dir = (game.turn == WHITE) ? 1 : -1;
    Bitboard double_step_rank = (game.turn == WHITE) ? RANK_3 : RANK_6;
    Bitboard promotion_rank = (game.turn == WHITE) ? RANK_8 : RANK_1;
    uint8_t attack_left_shift = (game.turn == WHITE) ? 7 : 9;
    uint8_t attack_right_shift = (game.turn == WHITE) ? 9 : 7;

    Bitboard pawns = game.pieces[turn_player][PAWN];
    Bitboard pawns_steps = shift(pawns, 8, dir) &  ~game.all;
    Bitboard pawns_double_steps = shift(pawns_steps & double_step_rank, 8, dir) & ~game.all;
    Bitboard pawns_attacks_left = shift(pawns & ~FILE_H, attack_left_shift, dir) & game.occupied[opponent_player];
    Bitboard pawns_attacks_right = shift(pawns & ~FILE_A, attack_right_shift, dir) & game.occupied[opponent_player];
    Bitboard pawns_en_passant_left = shift(pawns & ~FILE_H, attack_left_shift, dir) & game.en_passant;
    Bitboard pawns_en_passant_right = shift(pawns & ~FILE_A, attack_right_shift, dir) & game.en_passant;

    Bitboard move_mask = 1ULL;

    for(int block=0; block<64; block++){
        if(move_mask & pawns_steps){
            Bitboard destination = move_mask & pawns_steps;
            
            Move move;
            move.color = turn_player;
            move.piece_type = PAWN;
            move.to = destination;
            move.from = shift(destination, 8, -1*dir);
            move.is_promotion = destination & promotion_rank;
            move.promotion = QUEEN;

            if(isLegalMove(game, move)){
                moves.push_back(move);
            }
        }

        if(move_mask & pawns_double_steps){
            Bitboard destination = move_mask & pawns_double_steps;
            
            Move move;
            move.color = turn_player;
            move.piece_type = PAWN;
            move.to = destination;
            move.from = shift(destination, 16, -1*dir);
            move.pawn_double_push = true;

            if(isLegalMove(game, move)){
                moves.push_back(move);
            }
        }

        if(move_mask & pawns_attacks_left){
            Bitboard destination = move_mask & pawns_attacks_left;
            
            Move move;
            move.color = turn_player;
            move.piece_type = PAWN;
            move.to = destination;
            move.from = shift(destination, attack_left_shift, -1*dir);            
            move.capture = true;
            move.is_promotion = destination & promotion_rank;
            move.promotion = QUEEN;

            if(isLegalMove(game, move)){
                moves.push_back(move);
            }
        }

        if(move_mask & pawns_attacks_right){
            Bitboard destination = move_mask & pawns_attacks_right;

            Move move;
            move.color = turn_player;
            move.piece_type = PAWN;
            move.to = destination;
            move.from = shift(destination, attack_right_shift, -1*dir);
            move.capture = true;
            move.is_promotion = destination & promotion_rank;
            move.promotion = QUEEN;

            if(isLegalMove(game, move)){
                moves.push_back(move);
            }
        }

        if(move_mask & pawns_en_passant_left){
            Bitboard destination = move_mask & pawns_en_passant_left;

            Move move;
            move.color = turn_player;
            move.piece_type = PAWN;
            move.to = destination;
            move.from = shift(destination, attack_left_shift, -1*dir);
            move.capture = true;
            move.en_passant = true;
            
            if(isLegalMove(game, move)){
                moves.push_back(move);
            }
        }

        if(move_mask & pawns_en_passant_right){
            Bitboard destination = move_mask & pawns_en_passant_right;

            Move move;
            move.color = turn_player;
            move.piece_type = PAWN;
            move.to = destination;
            move.from = shift(destination, attack_right_shift, -1*dir);
            move.capture = true;
            move.en_passant = true;
            
            if(isLegalMove(game, move)){
                moves.push_back(move);
            }
        }

        move_mask <<= 1ULL;
    }
}
