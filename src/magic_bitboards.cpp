#include "magic_bitboards.h"
#include <iostream>


Bitboard ** alloc_bitboard_matrix(int rows, int cols){
    Bitboard ** res = new Bitboard*[rows];
    for(int i = 0; i < rows; i++){
        res[i] = new Bitboard[cols];
    }
    return res;
}

std::vector<Bitboard> all_possible_blockers(Bitboard mask){
    int num_blockers = 1 << countBits(mask);
    std::vector<Bitboard> res;
    for(int i = 0; i < num_blockers; i++){
        Bitboard blockers = 0;
        Bitboard index = i;
        Bitboard temp_mask = mask;
        for(int k = 0; k < 64 && index != 0; k++){
            if(index & 1){
                blockers |= temp_mask & -temp_mask;
            }
            temp_mask &= temp_mask - 1;
            index >>= 1;
        }
        res.push_back(blockers);
    }
    return res;
}

uint16_t magic_hash(uint64_t number, uint64_t magic_number, uint8_t digits){
    return (number * magic_number) >> (64 - digits);
}

bool is_magic(uint64_t candidate, const std::vector<Bitboard>& indexes, const std::vector<Bitboard>& outputs, uint8_t digits){
    int len = indexes.size();
    int max_val = 1ULL << digits;
    std::vector<bool> used(max_val, false);
    std::vector<Bitboard> value(max_val, 0);

    for(int i=0; i<len; i++){
        uint16_t hash_val = magic_hash(indexes[i], candidate, digits);
        if(used[hash_val] == false){
            used[hash_val] = true;
            value[hash_val] = outputs[i];
        } else if(value[hash_val] != outputs[i]){
            /* Collision of two indexes that map to two different outputs */
            return false;
        }
    }

    return true;
}

void setup_magic_db(Bitboard** db, int row, uint64_t magic_number, const std::vector<Bitboard>& indexes, const std::vector<Bitboard>& outputs, uint8_t digits){
    for(int i=0; i<indexes.size(); i++){
        db[row][magic_hash(indexes[i], magic_number, digits)] = outputs[i];
    }
    return;
}
