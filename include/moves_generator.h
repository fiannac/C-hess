#ifndef MOVES_GENERATOR_H
#define MOVES_GENERATOR_H

#include "chess_types.h"
#include "game.h"
#include <list>
#include <iostream>
#include <unordered_map>

class MovesGenerator {
private:
    Bitboard knight_masks[64];
    Bitboard king_masks[64];

    Bitboard rook_masks[64];
    Bitboard bishop_masks[64];

    Bitboard rook_magic_numbers[64];
    Bitboard**  rookDB;
    uint8_t rook_db_digits;

    Bitboard bishop_magic_numbers[64];
    Bitboard** bishopDB;
    uint8_t bishop_db_digits;

    //change to const &
    void generatePawnMoves(const Game& game, std::list<Move> &moves);
    void generateKnightMoves(const Game& game, std::list<Move> &moves);
    void generateBishopMoves(const Game& game, std::list<Move> &moves);
    void generateRookMoves(const Game& game, std::list<Move> &moves);
    void generateQueenMoves(const Game& game, std::list<Move> &moves);
    void generateKingMoves(const Game& game, std::list<Move> &moves);
    bool isLegalMove(const Game& game, Move move);

    void inizializeKnightMasks();
    void inizializeKingMasks();
    void inizializeRookMasks();
    void inizializeBishopMasks();

    void setRookMagicBitboard();
    void setBishopMagicBitboard();
public:

    MovesGenerator();
    std::list<Move> generateMoves(const Game& game);
};



#endif
