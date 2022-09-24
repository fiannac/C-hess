#ifndef MOVES_GENERATOR_H
#define MOVES_GENERATOR_H

#include "chess_types.h"
#include <list>
#include <iostream>
#include <unordered_map>

class MovesGenerator {
private:
    std::unordered_map<Bitboard, Bitboard> KNIGHT_PATTERNS;
    std::unordered_map<Bitboard, Bitboard> KING_PATTERNS;
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
public:

    MovesGenerator();
    std::list<Move> generateMoves(Game game);
};



#endif