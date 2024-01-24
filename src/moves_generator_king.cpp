#include "moves_generator.h"


void MovesGenerator::inizializeKingMasks(){
    Bitboard index = 1ULL;
    for(int i=0;i<64;i++){
        king_masks[i] = 
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
    Bitboard king_moves = this->king_masks[getBitIndex(king)] & ~game.occupied[turn_player];

    while(king_moves){
        Move move;
        move.color = turn_player;
        move.piece_type = KING;
        move.to = king_moves & -king_moves;
        move.from = king;
        move.capture = game.occupied[opponent_player] & move.to;

        if(isLegalMove(game, move)){
            moves.push_back(move);
        }
        king_moves &= king_moves - 1;
    }

    Bitboard king_square = white_playing ? E1 : E8;
    Bitboard rook_left_square = white_playing ? A1 : A8;
    Bitboard blocking_left = white_playing ? (B1 | C1 | D1) : (B8 | C8 | D8);
    Bitboard rook_right_square = white_playing ? H1 : H8;
    Bitboard blocking_right = white_playing ? (F1 | G1) : (F8 | G8);

    Bitboard move_to_left = white_playing ? C1 : C8;
    Bitboard move_to_right = white_playing ? G1 : G8;

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
        move.castling = true;

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
        move.castling = true;

        if(isLegalMove(game, move)){
            moves.push_back(move);
        }
    }
}
