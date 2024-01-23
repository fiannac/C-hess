#include "chess_types.h"
#include "game.h"
#include "moves_generator.h"
#include <list>
#include <iostream>
#include <string.h>
#include <string>


#define DEFAULT_BOARD "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1"


void game_turn(Game& game, MovesGenerator& moves_generator){
    std::list<Move> moves = moves_generator.generateMoves(game);
    
    for (auto move : moves) {
        std::cout << move.to_string() << ", ";
    }
    std::cout << std::endl;

    std::string move_str;
    Move to_do;
    bool valid_move = false;
    do{
        std::cout << "Move: ";
        std::cin >> move_str;
        std::cout << std::endl;

        for(auto move: moves){ 
            if(move_str == move.to_string()){
                to_do = move;
                valid_move = true;
            }
        }
    } while(!valid_move);

    game.make_move(to_do);
}


int main(int argc, char** argv){
    char* board = nullptr;
    if (argc == 1){
        board = new char[strlen(DEFAULT_BOARD) + 1];
        strcpy(board, DEFAULT_BOARD);
    } else {
        board = argv[1];
    }
    MovesGenerator moves_generator;
    Game game(board);

    while(1){
        std::cout << game.to_string() << std::endl;
        game_turn(game, moves_generator);
    }

    return 0;
}
