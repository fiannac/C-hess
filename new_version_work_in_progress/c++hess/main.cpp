#include "chess_types.h"
#include "moves_generator.h"
#include <list>
#include <iostream>
#include <string>

int main(){
    Game game("rnbkqbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBKQBNR w - - 0 1");
    std::cout << game.to_string() << std::endl;
    MovesGenerator moves_generator;

    for(int i=0;i<10;i++){
        
        std::list<Move> moves = moves_generator.generateMoves(game);
        if(game.turn == WHITE){
            std::cout << "White!" << std::endl;
        } else {
            std::cout << "Black!" << std::endl;
        }
        for (Move move : moves) {
            std::cout << move.to_string() << std::endl;
        }
        game.make_move(*moves.begin());
        std::cout << std::endl;
        std::cout << "Move made: " << (*moves.begin()).to_string() << std::endl;
        std::cout << game.to_string() << std::endl;
    }

    while(1);
    return 0;
}

