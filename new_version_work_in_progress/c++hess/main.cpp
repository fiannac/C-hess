#include "chess_types.h"
#include "moves_generator.h"
#include <list>
#include <iostream>
#include <string>

int main(){
    Game game("R6P/3k4/p7/6PP/3p2PK/6pp/8/8 w - - 0 1");
    MovesGenerator moves_generator;
    std::cout << game.to_string() << std::endl;
    try{
        std::list<Move> moves = moves_generator.generateMoves(game);
        for (Move move : moves) {
            std::cout << move.to_string() << std::endl;
        }
    }
    catch (const std::exception& e) {
        std::cout << e.what() << std::endl;
    }

    while(1);
    return 0;
}