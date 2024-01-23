#include "moves_generator.h"
#include <vector>
#include "magic_bitboards.h"


void MovesGenerator::inizializeBishopMasks(){
    for(int i=0;i<64;i++){
        Bitboard mask = 0ULL;
        Bitboard index = 1ULL << i;
        Bitboard indexCopy = index;
        for(int j = 0; j < 8; j++){
            mask |= index;
            index = index & NOT(FILE_H);
            index = index << 7;
        }
        index = 1ULL << i;
        for(int j = 0; j < 8; j++){
            mask |= index;
            index = index & NOT(FILE_A);
            index = index << 9;
        }
        index = 1ULL << i;
        for(int j = 0; j < 8; j++){
            mask |= index;
            index = index & NOT(FILE_A);
            index = index >> 7;
        }
        index = 1ULL << i;
        for(int j = 0; j < 8; j++){
            mask |= index;
            index = index & NOT(FILE_H);
            index = index >> 9;
        }
        bishop_masks[i] = mask & NOT(indexCopy) & NOT(FILE_A) & NOT(FILE_H) & NOT(RANK_1) & NOT(RANK_8);
    }
}


std::vector<Bitboard> generate_bishop_movements(const std::vector<Bitboard>& blockers, const int index){
    Bitboard pos = 1ULL << index;
    std::vector<Bitboard> movements;
    for(auto blocker: blockers){
        Bitboard attacks = 0;
        Bitboard temp = pos;
        while(temp){
            temp &= NOT(FILE_H);
            temp <<= 7;
            attacks |= temp;
            temp &= NOT(blocker);
        }
        temp = pos;
        while(temp){
            temp &= NOT(FILE_A);
            temp <<= 9;
            attacks |= temp;
            temp &= NOT(blocker);
        }
        temp = pos;
        while(temp){
            temp &= NOT(FILE_H);
            temp >>= 9;
            attacks |= temp;
            temp &= NOT(blocker);
        }
        temp = pos;
        while(temp){
            temp &= NOT(FILE_A);
            temp >>= 7;
            attacks |= temp;
            temp &= NOT(blocker);
        }
        movements.push_back(attacks);  
    }
    return movements;
} 


void MovesGenerator::setBishopMagicBitboard(){
    rook_db_digits = 10;
    bishopDB = alloc_bitboard_matrix(64, 1ULL << rook_db_digits);
    uint64_t candidate;

    for(int i = 0; i<64; i++){
        bool found_magic = false;
        std::vector<Bitboard> blockers = all_possible_blockers(bishop_masks[i]);
        std::vector<Bitboard> movements = generate_bishop_movements(blockers, i);
        while(!found_magic){
            candidate = randomU64() & randomU64() & randomU64();
            if(is_magic(candidate, blockers, movements, rook_db_digits)){
                bishop_magic_numbers[i] = candidate;
                setup_magic_db(bishopDB, i, candidate, blockers, movements, rook_db_digits);
                found_magic = true;
            }
        }
    }
}


void MovesGenerator::generateBishopMoves(const Game& game, std::list<Move> &moves){
    Color turn_player = game.turn;
    Color opponent_player = (game.turn == WHITE) ? BLACK : WHITE;
    
    Bitboard bishops = game.pieces[turn_player][BISHOP];
    while(bishops){
        Bitboard bishop = bishops & -bishops;
        int index = getBitIndex(bishop);

        Bitboard bishop_moves = bishopDB[index][
            magic_hash(game.all & bishop_masks[index], bishop_magic_numbers[index], rook_db_digits)
        ] & ~game.occupied[turn_player];

        while(bishop_moves){
            Move move;
            move.color = turn_player;
            move.piece_type = BISHOP;
            move.to = bishop_moves & -bishop_moves;
            move.from = bishop;
            move.capture = game.occupied[opponent_player] & move.to;
            move.en_passant = false;
            move.castling = false;
            move.is_promotion = false;

            if(isLegalMove(game, move)){
                moves.push_back(move);
            }
            bishop_moves &= bishop_moves - 1;
        }
        bishops &= bishops - 1;
    }
}
