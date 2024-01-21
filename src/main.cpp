#include "chess_types.h"
#include "game.h"
#include "moves_generator.h"
#include <list>
#include <iostream>
#include <string>

int main(){
    Game game("rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w - - 0 1");
    std::cout << game.to_string() << std::endl;
    MovesGenerator moves_generator;

    for(int i=0;i<10;i++){
        
        std::list<Move> moves = moves_generator.generateMoves(game);
    
        int num = 0;
        for (Move move : moves) {
            std::cout << num << ") " << move.to_string() << std::endl;
            num++;
        }
        int to_do;
        std::cout << "Inserisci mossa: ";
        std::cin >> to_do;
        std::cout << std::endl;
        auto pointer = moves.begin();
        std::advance(pointer, to_do);
        game.make_move(*pointer);
        std::cout << "Move made: " << (*moves.begin()).to_string() << std::endl;
        std::cout << game.to_string() << std::endl;
    }

    while(1);
    return 0;
}
