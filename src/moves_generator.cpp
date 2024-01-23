#include "moves_generator.h"
#include "chess_types.h"
#include "game.h"
#include <iostream>
#include <unordered_map>
#include <vector>


std::list<Move> MovesGenerator::generateMoves(const Game& game){
    std::list<Move> moves;
    generatePawnMoves(game, moves);
    generateKnightMoves(game, moves);
    generateBishopMoves(game, moves);
    generateRookMoves(game, moves);
    generateQueenMoves(game, moves);
    generateKingMoves(game, moves);

    return moves;
}

MovesGenerator::MovesGenerator(){
    inizializeKnightMasks();
    inizializeKingMasks();
    inizializeRookMasks();
    inizializeBishopMasks();

    setRookMagicBitboard();
    setBishopMagicBitboard();
}

void MovesGenerator::generateQueenMoves(const Game& game, std::list<Move> &moves){
        if(game.turn == WHITE){
        Bitboard white_queens = game.pieces[WHITE][QUEEN];
        while(white_queens){
            Bitboard white_queen = white_queens & -white_queens;
            int index = getBitIndex(white_queen);
            
            Bitboard white_queen_moves = 
                (rookDB[index][magicHash(game.all & ROOK_MASKS[index], ROOK_MAGIC_NUMBERS[index], ROOK_SHIFT_BITS)] |
                bishopDB[index][magicHash(game.all & BISHOP_MASKS[index], BISHOP_MAGIC_NUMBERS[index], BISHOP_SHIFT_BITS)]) & 
                ~game.occupied[WHITE];

            while(white_queen_moves){
                Move move;
                move.color = WHITE;
                move.piece_type = QUEEN;
                move.to = white_queen_moves & -white_queen_moves;
                move.from = white_queen;
                move.capture = game.occupied[BLACK] & move.to;
                move.en_passant = false;
                move.castling = false;
                move.is_promotion = false;

                if(isLegalMove(game, move)){
                    moves.push_back(move);
                }
                white_queen_moves &= white_queen_moves - 1;
            }
            white_queens &= white_queens - 1;
        }
    } else if(game.turn == BLACK){
        Bitboard black_queens = game.pieces[BLACK][QUEEN];
        while(black_queens){
            Bitboard black_queen = black_queens & -black_queens;
            int index = getBitIndex(black_queen);

            Bitboard black_queen_moves = 
                (rookDB[index][magicHash(game.all & ROOK_MASKS[index], ROOK_MAGIC_NUMBERS[index], ROOK_SHIFT_BITS)] |
                bishopDB[index][magicHash(game.all & BISHOP_MASKS[index], BISHOP_MAGIC_NUMBERS[index], BISHOP_SHIFT_BITS)]) & 
                ~game.occupied[BLACK];

            while(black_queen_moves){
                Move move;
                move.color = BLACK;
                move.piece_type = QUEEN;
                move.to = black_queen_moves & -black_queen_moves;
                move.from = black_queen;
                move.capture = game.occupied[WHITE] & move.to;
                move.en_passant = false;
                move.castling = false;
                move.is_promotion = false;

                if(isLegalMove(game, move)){
                    moves.push_back(move);
                }
                black_queen_moves &= black_queen_moves - 1;
            }
            black_queens &= black_queens - 1;
        }
    }
}

bool MovesGenerator::isLegalMove(const Game& game, Move move){
    return true;
}
