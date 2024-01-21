#ifndef MOVES_GENERATOR_H
#define MOVES_GENERATOR_H

#include "chess_types.h"
#include <list>
#include <iostream>
#include <unordered_map>

class MovesGenerator {
private:
    Bitboard KNIGHT_PATTERNS[64];
    Bitboard KING_PATTERNS[64];

    Bitboard ROOK_MASKS[64];
    Bitboard BISHOP_MASKS[64];

    Bitboard ROOK_MAGIC_NUMBERS[64];
    Bitboard**  rookDB;

    Bitboard BISHOP_MAGIC_NUMBERS[64];
    Bitboard** bishopDB;

    int ROOK_SHIFT_BITS;
    int BISHOP_SHIFT_BITS;
    //change to const &
    void generatePawnMoves(const Game& game, std::list<Move> &moves);
    void generateKnightMoves(const Game& game, std::list<Move> &moves);
    void generateBishopMoves(const Game& game, std::list<Move> &moves);
    void generateRookMoves(const Game& game, std::list<Move> &moves);
    void generateQueenMoves(const Game& game, std::list<Move> &moves);
    void generateKingMoves(const Game& game, std::list<Move> &moves);
    bool isLegalMove(const Game& game, Move move);

    void inizializeKnightPatterns();
    void inizializeKingPatterns();
    void inizializeRookMasks();
    void inizializeBishopMasks();

    void setRookMagicBitboard();
    void setBishopMagicBitboard();
public:

    MovesGenerator();
    std::list<Move> generateMoves(const Game& game);
};



#endif
