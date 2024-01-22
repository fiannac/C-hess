#include "chess_types.h"
#include "game.h"
#include "moves_generator.h"
#include <list>
#include <iostream>
#include <string.h>

#define DEFAULT_BOARD "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w - - 0 1"

int main(int argc, char** argv){
    char* board = nullptr;
    if (argc == 1){
        board = new char[strlen(DEFAULT_BOARD) + 1];
        strcpy(board, DEFAULT_BOARD);
    } else {
        board = argv[1];
    }
    Game game(board);
    std::cout << game.to_string() << std::endl;
    MovesGenerator moves_generator;

    while(1){
        std::list<Move> moves = moves_generator.generateMoves(game);
    
        int num = 0;
        for (Move move : moves) {
            std::cout << num << ") " << move.to_string() << std::endl;
            num++;
        }
        int to_do;
        std::cout << "Move (index): ";
        std::cin >> to_do;
        std::cout << std::endl;
        auto pointer = moves.begin();
        std::advance(pointer, to_do);
        game.make_move(*pointer);
        std::cout << "Move made: " << (*moves.begin()).to_string() << std::endl;
        std::cout << game.to_string() << std::endl;
    }

    return 0;
}
