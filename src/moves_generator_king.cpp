#include "moves_generator.h"


void MovesGenerator::inizializeKingMasks(){
    Bitboard index = 1ULL;
    for(int i=0;i<64;i++){
        KING_MASKS[i] = 
            (index & ~FILE_H) >> 1 |
            (index & ~FILE_H) >> 9 |
            (index & ~RANK_1) >> 8 |
            (index & ~FILE_A) >> 7 |
            (index & ~FILE_A) << 1 |
            (index & ~FILE_A) << 9 |
            (index & ~RANK_8) << 8 |
            (index & ~FILE_H) << 7;
        index <<= 1;
    }
}

void MovesGenerator::generateKingMoves(const Game& game, std::list<Move> &moves){
    Color turn_player = game.turn;
    bool white_playing = (turn_player == WHITE);
    Color opponent_player = white_playing ? BLACK : WHITE;
    
    Bitboard king = game.pieces[turn_player][KING];
    Bitboard king_moves = this->KING_MASKS[getBitIndex(king)] & ~game.occupied[turn_player];

    while(king_moves){
        Move move;
        move.color = turn_player;
        move.piece_type = KING;
        move.to = king_moves & -king_moves;
        move.from = king;
        move.capture = game.occupied[opponent_player] & move.to;
        move.en_passant = false;
        move.castling = false;
        move.is_promotion = false;

        if(isLegalMove(game, move)){
            moves.push_back(move);
        }
        king_moves &= king_moves - 1;
    }

    Bitboard king_square = white_playing ? 1ULL << E1 : 1ULL << E8;
    Bitboard rook_left_square = white_playing ? 1ULL << A1 : 1ULL << A8;
    Bitboard blocking_left = white_playing ? (1ULL << B1 | 1ULL << C1 | 1ULL << D1) : (1ULL << B8 | 1ULL << C8 | 1ULL << D8);
    Bitboard rook_right_square = white_playing ? 1ULL << H1 : 1ULL << H8;
    Bitboard blocking_right = white_playing ? (1ULL << F1 | 1ULL << G1) : (1ULL << F8 | 1ULL << G8);


    Bitboard move_to_left = white_playing ? 1ULL << C1 : 1ULL << C8;
    Bitboard move_to_right = white_playing ? 1ULL << G1 : 1ULL << G8;

    bool king_in_place = (game.pieces[turn_player][KING] & king_square) != 0ULL;
    
    bool rook_left_in_place = (game.pieces[turn_player][ROOK] & rook_left_square) != 0ULL;
    bool no_blocking_left = (game.all & blocking_left) == 0ULL;

    bool rook_right_in_place = (game.pieces[turn_player][ROOK] & rook_right_square) != 0ULL;
    bool no_blocking_right = (game.all & blocking_right) == 0ULL;

    bool can_castle_left = white_playing ? game.white_can_castle_queenside : game.black_can_castle_queenside;
    bool can_castle_right = white_playing ? game.white_can_castle_kingside : game.black_can_castle_kingside;


    if(
        can_castle_left &&
        no_blocking_left &&
        rook_left_in_place &&
        king_in_place
    ){
        Move move;
        move.color = turn_player;
        move.piece_type = KING;
        move.to = move_to_left;
        move.from = king_square;
        move.capture = false;
        move.en_passant = false;
        move.castling = true;
        move.is_promotion = false;

        if(isLegalMove(game, move)){
            moves.push_back(move);
        }
    }

    if(
        can_castle_right &&
        no_blocking_right &&
        rook_right_in_place &&
        king_in_place
    ){
        Move move;
        move.color = WHITE;
        move.piece_type = KING;
        move.to = move_to_right;
        move.from = king_square;
        move.capture = false;
        move.en_passant = false;
        move.castling = true;
        move.is_promotion = false;

        if(isLegalMove(game, move)){
            moves.push_back(move);
        }
    }
}
