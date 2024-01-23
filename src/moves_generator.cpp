#include "moves_generator.h"
#include "chess_types.h"
#include "game.h"
#include <iostream>
#include <unordered_map>
#include <vector>


std::list<Move> MovesGenerator::generateMoves(const Game& game){
    std::list<Move> moves;
    generatePawnMoves(game, moves);
    generateKnightMoves(game, moves);
    generateBishopMoves(game, moves);
    generateRookMoves(game, moves);
    generateQueenMoves(game, moves);
    generateKingMoves(game, moves);

    return moves;
}

MovesGenerator::MovesGenerator(){
    inizializeKnightMasks();
    inizializeKingMasks();
    inizializeRookMasks();
    inizializeBishopMasks();

    setRookMagicBitboard();
    setBishopMagicBitboard();
}

bool MovesGenerator::isLegalMove(const Game& game, Move move){
    return true;
}
