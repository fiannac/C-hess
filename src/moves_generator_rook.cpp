#include "moves_generator.h"
#include "chess_types.h"
#include "magic_bitboards.h"
#include <vector>
#include <iostream>


void MovesGenerator::inizializeRookMasks(){
    Bitboard files[8];
    Bitboard ranks[8];

    for(int i=0;i<8;i++){
        files[i] = 0ULL;
        ranks[i] = 0ULL;
    }

    for(int i=0;i<8;i++){
        if(i==0){
            files[i] = 0x8080808080808080ULL;
            ranks[i] = 0xFFULL;
        } else {
            files[i] = files[i-1] >> 1;
            ranks[i] = ranks[i-1] << 8;
        }
    }

    for(int i=0;i<64;i++){
        int file = 7 - i % 8;
        int rank = i / 8;
    
        ROOK_MASKS[i] = (files[file] | ranks[rank]);
        
        if(file != 0)
            ROOK_MASKS[i] &= ~FILE_A;
        if(file != 7)
            ROOK_MASKS[i] &= ~FILE_H;
        if(rank != 0)
            ROOK_MASKS[i] &= ~RANK_1;
        if(rank != 7)
            ROOK_MASKS[i] &= ~RANK_8;
        
        ROOK_MASKS[i] &= NOT(1ULL << i);
    }
}


std::vector<Bitboard> generate_rook_movements(const std::vector<Bitboard>& blockers, const int index){
    Bitboard pos = 1ULL << index;
    std::vector<Bitboard> movements;
    for(auto blocker: blockers){
        Bitboard attacks = 0;
        Bitboard temp = pos;
        while(temp){
            temp &= NOT(FILE_A);
            temp <<= 1;
            attacks |= temp;
            temp &= NOT(blocker);
        }
        temp = pos;
        while(temp){
            temp &= NOT(FILE_H);
            temp >>= 1;
            attacks |= temp;
            temp &= NOT(blocker);
        }
        temp = pos;
        while(temp){
            temp &= NOT(RANK_8);
            temp <<= 8;
            attacks |= temp;
            temp &= NOT(blocker);
        }
        temp = pos;
        while(temp){
            temp &= NOT(RANK_1);
            temp >>= 8;
            attacks |= temp;
            temp &= NOT(blocker);
        }
        movements.push_back(attacks);  
    }
    return movements;
} 


void MovesGenerator::setRookMagicBitboard(){
    rookDB = alloc_bitboard_matrix(64, 4096);
    uint64_t candidate;

    for(int i = 0; i<64; i++){
        bool found_magic = false;
        std::vector<Bitboard> blockers = all_possible_blockers(ROOK_MASKS[i]);
        std::vector<Bitboard> movements = generate_rook_movements(blockers, i);
        while(!found_magic){
            candidate = randomU64() & randomU64() & randomU64();
            if(is_magic(candidate, blockers, movements, 12)){
                ROOK_MAGIC_NUMBERS[i] = candidate;
                setup_magic_db(rookDB, i, candidate, blockers, movements, 12);
                found_magic = true;
            }
        }
    }
}


void MovesGenerator::generateRookMoves(const Game& game, std::list<Move> &moves){
    Color turn_player = game.turn;
    Color opponent_player = (game.turn == WHITE) ? BLACK : WHITE;
    
    Bitboard rooks = game.pieces[turn_player][ROOK];
    while(rooks){
        Bitboard rook = rooks & -rooks;
        int index = getBitIndex(rook);

        Bitboard rook_moves = rookDB[index][
            magic_hash(game.all & ROOK_MASKS[index], ROOK_MAGIC_NUMBERS[index], 12)
        ] & ~game.occupied[turn_player];

        while(rook_moves){
            Move move;
            move.color = turn_player;
            move.piece_type = ROOK;
            move.to = rook_moves & -rook_moves;
            move.from = rook;
            move.capture = game.occupied[opponent_player] & move.to;
            move.en_passant = false;
            move.castling = false;
            move.is_promotion = false;

            if(isLegalMove(game, move)){
                moves.push_back(move);
            }
            rook_moves &= rook_moves - 1;
        }
        rooks &= rooks - 1;
    }

}

