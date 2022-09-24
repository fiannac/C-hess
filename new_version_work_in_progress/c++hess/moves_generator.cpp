#include "moves_generator.h"
#include "chess_types.h"
#include <iostream>

std::list<Move> MovesGenerator::generateMoves(Game game){
    //generate moves from bitboards
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
    inizializeKnightPatterns();
    inizializeKingPatterns();
}

void MovesGenerator::inizializeKnightPatterns(){
    Bitboard index = 1ULL;
    for(int i=0;i<64;i++){
        KNIGHT_PATTERNS[index] = 
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

void MovesGenerator::inizializeKingPatterns(){
    Bitboard index = 1ULL;
    for(int i=0;i<64;i++){
        KING_PATTERNS[index] = 
            (index & ~FILE_H) >> 1 |
            (index & ~FILE_H) >> 9 |
            (index & ~RANK_1) >> 8 |
            (index & ~FILE_A) >> 7 |
            (index & ~FILE_A) << 1 |
            (index & ~FILE_A) << 9 |
            (index & ~RANK_8) << 8 |
            (index & ~FILE_H)  << 7;
        index <<= 1;
    }
}

void MovesGenerator::generatePawnMoves(Game game, std::list<Move> &moves){
    if(game.turn == WHITE){
        //generate white pawn moves
        Bitboard white_pawns = game.pieces[WHITE][PAWN];
        Bitboard white_pawns_steps = (white_pawns << 8) &  ~game.all;
        Bitboard white_pawns_double_steps = ((white_pawns_steps & RANK_3) << 8) & ~game.all;
        Bitboard white_pawns_attacks_left = ((white_pawns & ~FILE_A) << 7) & game.occupied[BLACK];
        Bitboard white_pawns_attacks_right = ((white_pawns & ~FILE_H) << 9) & game.occupied[BLACK];
        Bitboard white_pawns_en_passant_left = ((white_pawns & ~FILE_A) << 7) & game.en_passant;
        Bitboard white_pawns_en_passant_right = ((white_pawns & ~FILE_H) << 9) & game.en_passant;

        while(white_pawns_steps){
            Move move;
            move.color = WHITE;
            move.piece_type = PAWN;
            move.to = white_pawns_steps & -white_pawns_steps;
            move.from = move.to >> 8;
            move.capture = false;
            move.en_passant = false;
            move.castling = false;
            if(move.to & RANK_8){
                move.is_promotion = true;
                move.promotion = QUEEN;
            }
            else{
                move.is_promotion = false;
            }

            if(isLegalMove(game, move)){
                moves.push_back(move);
            }

            white_pawns_steps &= white_pawns_steps - 1;
        }

        while(white_pawns_double_steps){
            Move move;
            move.color = WHITE;
            move.piece_type = PAWN;
            move.to = white_pawns_double_steps & -white_pawns_double_steps;
            move.from = move.to >> 16;
            move.capture = false;
            move.en_passant = false;
            move.castling = false;
            move.is_promotion = false;

            if(isLegalMove(game, move)){
                moves.push_back(move);
            }

            white_pawns_double_steps &= white_pawns_double_steps - 1;
        }

        while(white_pawns_attacks_left){
            Move move;
            move.color = WHITE;
            move.piece_type = PAWN;
            move.to = white_pawns_attacks_left & -white_pawns_attacks_left;
            move.from = move.to >> 7;
            move.capture = true;
            move.en_passant = false;
            move.castling = false;
            if(move.to & RANK_8){
                move.is_promotion = true;
                move.promotion = QUEEN;
            }
            else{
                move.is_promotion = false;
            }

            if(isLegalMove(game, move)){
                moves.push_back(move);
            }

            white_pawns_attacks_left &= white_pawns_attacks_left - 1;
        }

        while(white_pawns_attacks_right){
            Move move;
            move.color = WHITE;
            move.piece_type = PAWN;
            move.to = white_pawns_attacks_right & -white_pawns_attacks_right;
            move.from = move.to >> 9;
            move.capture = true;
            move.en_passant = false;
            move.castling = false;
            if(move.to & RANK_8){
                move.is_promotion = true;
                move.promotion = QUEEN;
            }
            else{
                move.is_promotion = false;
            }

            if(isLegalMove(game, move)){
                moves.push_back(move);
            }

            white_pawns_attacks_right &= white_pawns_attacks_right - 1;
        }

        //en passant when you will set that be carefull!

        if(white_pawns_en_passant_left){
            Move move;
            move.color = WHITE;
            move.piece_type = PAWN;
            move.to = white_pawns_en_passant_left & -white_pawns_en_passant_left;
            move.from = move.to >> 7;
            move.capture = true;
            move.en_passant = true;
            move.castling = false;
            move.is_promotion = false;
            if(isLegalMove(game, move)){
                moves.push_back(move);
            }
        }

        if(white_pawns_en_passant_right){
            Move move;
            move.color = WHITE;
            move.piece_type = PAWN;
            move.to = white_pawns_en_passant_right & -white_pawns_en_passant_right;
            move.from = move.to >> 9;
            move.capture = true;
            move.en_passant = true;
            move.castling = false;
            move.is_promotion = false;

            if(isLegalMove(game, move)){
                moves.push_back(move);
            }
        }

        
    } else if(game.turn == BLACK){ 
        //generate black pawn moves
        Bitboard black_pawns = game.pieces[BLACK][PAWN];
        Bitboard black_pawns_steps = (black_pawns >> 8) & ~game.all;
        Bitboard black_pawns_double_steps = ((black_pawns_steps & RANK_6) >> 8) & ~game.all;
        Bitboard black_pawns_attacks_left = ((black_pawns & ~FILE_A) >> 9) & game.occupied[WHITE];
        Bitboard black_pawns_attacks_right = ((black_pawns & ~FILE_H) >> 7) & game.occupied[WHITE];
        Bitboard black_pawns_en_passant_left = ((black_pawns & ~FILE_A) >> 9) & game.en_passant;
        Bitboard black_pawns_en_passant_right = ((black_pawns & ~FILE_H) >> 7) & game.en_passant;

        while(black_pawns_steps){
            Move move;
            move.color = BLACK;
            move.piece_type = PAWN;
            move.to = black_pawns_steps & -black_pawns_steps;
            move.from = move.to << 8;
            move.capture = false;
            move.en_passant = false;
            move.castling = false;
            if(move.to & RANK_1){
                move.is_promotion = true;
                move.promotion = QUEEN;
            }
            else{
                move.is_promotion = false;
            }

            if(isLegalMove(game, move)){
                moves.push_back(move);
            }

            black_pawns_steps &= black_pawns_steps - 1;
        }

        while(black_pawns_double_steps){
            Move move;
            move.color = BLACK;
            move.piece_type = PAWN;
            move.to = black_pawns_double_steps & -black_pawns_double_steps;
            move.from = move.to << 16;
            move.capture = false;
            move.en_passant = false;
            move.castling = false;
            move.is_promotion = false;

            if(isLegalMove(game, move)){
                moves.push_back(move);
            }

            black_pawns_double_steps &= black_pawns_double_steps - 1;
        }

        while(black_pawns_attacks_left){
            Move move;
            move.color = BLACK;
            move.piece_type = PAWN;
            move.to = black_pawns_attacks_left & -black_pawns_attacks_left;
            move.from = move.to << 9;
            move.capture = true;
            move.en_passant = false;
            move.castling = false;
            if(move.to & RANK_1){
                move.is_promotion = true;
                move.promotion = QUEEN;
            }
            else{
                move.is_promotion = false;
            }

            if(isLegalMove(game, move)){
                moves.push_back(move);
            }

            black_pawns_attacks_left &= black_pawns_attacks_left - 1;
        }

        while(black_pawns_attacks_right){
            Move move;
            move.color = BLACK;
            move.piece_type = PAWN;
            move.to = black_pawns_attacks_right & -black_pawns_attacks_right;
            move.from = move.to << 7;
            move.capture = true;
            move.en_passant = false;
            move.castling = false;
            if(move.to & RANK_1){
                move.is_promotion = true;
                move.promotion = QUEEN;
            }
            else{
                move.is_promotion = false;
            }

            if(isLegalMove(game, move)){
                moves.push_back(move);
            }

            black_pawns_attacks_right &= black_pawns_attacks_right - 1;
        }

        if(black_pawns_en_passant_left){
            Move move;
            move.color = BLACK;
            move.piece_type = PAWN;
            move.to = black_pawns_en_passant_left & -black_pawns_en_passant_left;
            move.from = move.to << 7;
            move.capture = true;
            move.en_passant = true;
            move.castling = false;
            move.is_promotion = false;

            if(isLegalMove(game, move)){
                moves.push_back(move);
            }

            black_pawns_en_passant_left &= black_pawns_en_passant_left - 1;
        }

        if(black_pawns_en_passant_right){
            Move move;
            move.color = BLACK;
            move.piece_type = PAWN;
            move.to = black_pawns_en_passant_right & -black_pawns_en_passant_right;
            move.from = move.to << 9;
            move.capture = true;
            move.en_passant = true;
            move.castling = false;
            move.is_promotion = false;

            if(isLegalMove(game, move)){
                moves.push_back(move);
            }

            black_pawns_en_passant_right &= black_pawns_en_passant_right - 1;
        }


    }
}

void MovesGenerator::generateKnightMoves(Game game, std::list<Move> &moves){
    
    if(game.turn == WHITE){
        Bitboard white_knights = game.pieces[WHITE][KNIGHT];
        while(white_knights){
            
            Bitboard white_knight = white_knights & -white_knights;
            Bitboard white_knight_moves = this->KNIGHT_PATTERNS[white_knight] & ~game.occupied[WHITE];
            while(white_knight_moves){
                Move move;
                move.color = WHITE;
                move.piece_type = KNIGHT;
                move.to = white_knight_moves & -white_knight_moves;
                move.from = white_knight;
                move.capture = game.occupied[BLACK] & move.to;
                move.en_passant = false;
                move.castling = false;
                move.is_promotion = false;
                move.check = false;

                if(isLegalMove(game, move)){
                    moves.push_back(move);
                }

                white_knight_moves &= white_knight_moves - 1;
            }
            white_knights &= white_knights - 1;
        }
    } else if(game.turn == BLACK){
        Bitboard black_knights = game.pieces[BLACK][KNIGHT];

        while(black_knights){
            Bitboard black_knight = black_knights & -black_knights;
            Bitboard black_knight_moves = this->KNIGHT_PATTERNS[black_knight] & ~game.occupied[BLACK];

            while(black_knight_moves){
                Move move;
                move.color = BLACK;
                move.piece_type = KNIGHT;
                move.to = black_knight_moves & -black_knight_moves;
                move.from = black_knight;
                move.capture = game.occupied[WHITE] & move.to;
                move.en_passant = false;
                move.castling = false;
                move.is_promotion = false;

                if(isLegalMove(game, move)){
                    moves.push_back(move);
                }

                black_knight_moves &= black_knight_moves - 1;
            }
            black_knights &= black_knights - 1;
        }
    }
}

void MovesGenerator::generateKingMoves(Game game, std::list<Move> &moves){
    if(game.turn == WHITE){
        std::cout << "White king moves" << std::endl;
        Bitboard white_king = game.pieces[WHITE][KING];
        Bitboard white_king_moves = this->KING_PATTERNS[white_king] & ~game.occupied[WHITE];
        while(white_king_moves){
            Move move;
            move.color = WHITE;
            move.piece_type = KING;
            move.to = white_king_moves & -white_king_moves;
            move.from = white_king;
            move.capture = game.occupied[BLACK] & move.to;
            move.en_passant = false;
            move.castling = false;
            move.is_promotion = false;

            if(isLegalMove(game, move)){
                moves.push_back(move);
            }
            white_king_moves &= white_king_moves - 1;
        }
    } else if(game.turn == BLACK){
        Bitboard black_king = game.pieces[BLACK][KING];
        Bitboard black_king_moves = this->KING_PATTERNS[black_king] & ~game.occupied[BLACK];
        while(black_king_moves){
            Move move;
            move.color = BLACK;
            move.piece_type = KING;
            move.to = black_king_moves & -black_king_moves;
            move.from = black_king;
            move.capture = game.occupied[WHITE] & move.to;
            move.en_passant = false;
            move.castling = false;
            move.is_promotion = false;

            if(isLegalMove(game, move)){
                moves.push_back(move);
            }
            black_king_moves &= black_king_moves - 1;
        }
    }
}

void MovesGenerator::generateBishopMoves(Game game, std::list<Move> &moves){

}

void MovesGenerator::generateRookMoves(Game game, std::list<Move> &moves){

}

void MovesGenerator::generateQueenMoves(Game game, std::list<Move> &moves){

}

bool MovesGenerator::isLegalMove(Game game, Move move){
    return true;
}