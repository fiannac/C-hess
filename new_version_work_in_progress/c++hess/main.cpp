#include "chess_types.h"
#include "moves_generator.h"
#include <list>
#include <iostream>
#include <string>

int main(){
    Game game("1B6/2P5/8/8/3K4/8/8/8 w - - 0 1");
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