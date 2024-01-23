#ifndef MAGIC_BITBOARDS_H
#define MAGIC_BITBOARDS_H

#include <vector>
#include "chess_types.h"

std::vector<Bitboard> all_possible_blockers(Bitboard bb);
uint16_t magic_hash(uint64_t number, uint64_t magic_number, uint8_t digits);
Bitboard ** alloc_bitboard_matrix(int rows, int cols);
bool is_magic(uint64_t candidate, const std::vector<Bitboard>& indexes, const std::vector<Bitboard>& outputs, uint8_t digits);
void setup_magic_db(Bitboard** db, int, uint64_t magic, const std::vector<Bitboard>& indexes, const std::vector<Bitboard>& outputs, uint8_t digits);

#endif
