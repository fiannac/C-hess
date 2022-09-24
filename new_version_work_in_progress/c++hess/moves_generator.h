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
    //change to const &
    void generatePawnMoves(Game game, std::list<Move> &moves);
    void generateKnightMoves(Game game, std::list<Move> &moves);
    void generateBishopMoves(Game game, std::list<Move> &moves);
    void generateRookMoves(Game game, std::list<Move> &moves);
    void generateQueenMoves(Game game, std::list<Move> &moves);
    void generateKingMoves(Game game, std::list<Move> &moves);
    bool isLegalMove(Game game, Move move);

    void inizializeKnightPatterns();
    void inizializeKingPatterns();
    void inizializeRookMasks();
    void inizializeBishopMasks();
public:

    MovesGenerator();
    std::list<Move> generateMoves(Game game);
};



#endif