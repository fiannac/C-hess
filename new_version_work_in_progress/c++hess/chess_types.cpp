#include "chess_types.h"
#include <string>
#include <iostream>

Game::Game(std::string fen) {
    Bitboard pos = 0x8000000000000000;
    int i = 0;

    // Initialize all bitboards to 0
    for (int color = 0; color < 2; color++) {
        for (int piece_type = 0; piece_type < 6; piece_type++) {
            pieces[color][piece_type] = 0;
        }
    }

    occupied[WHITE] = 0;
    occupied[BLACK] = 0;
    all = 0;


    for (i = 0; i < fen.length(); i++) {
        char c = fen[i];

        if (c == ' ') {
            break;
        } else if (c >= '1' && c <= '8') {
            pos >>= (c - '0');
        } else if(c == '/'){
            
        }
        else {
            Color color = (c >= 'a' && c <= 'z') ? BLACK : WHITE;
            PieceType piece_type;
            switch (c) {
            case 'P':
            case 'p':
                piece_type = PAWN;
                break;
            case 'N':
            case 'n':
                piece_type = KNIGHT;
                break;
            case 'B':
            case 'b':
                piece_type = BISHOP;
                break;
            case 'R':
            case 'r':
                piece_type = ROOK;
                break;
            case 'Q':
            case 'q':
                piece_type = QUEEN;
                break;
            case 'K':
            case 'k':
                piece_type = KING;
                break;
            }
            pieces[color][piece_type] |= pos;
            occupied[color] |= pos;
            all |= pos;
            pos >>= 1;
        }

    }
    i++;
    turn = (fen[i] == 'w') ? WHITE : BLACK;
    i += 2;
    
    while(fen[i] != ' ') {
        char c = fen[i];
        switch (c) {
        case 'K':
            white_can_castle_kingside = true;
            break;
        case 'Q':
            white_can_castle_queenside = true;
            break;
        case 'k':
            black_can_castle_kingside = true;
            break;
        case 'q':
            black_can_castle_queenside = true;
            break;
        }
        i++;
    }
    i++;
    en_passant = 0;
    en_passant_possible = false;
    if (fen[i] != '-') {
        int file = fen[i] - 'a';
        i++;
        int rank = fen[i] - '1';
        i++;
        en_passant = 0x8000000000000000 >> (file + 8 * rank);
        en_passant_possible = true;
    }
    i++;
    halfmove_clock = 0;
    while (fen[i] >= '0' && fen[i] <= '9') {
        halfmove_clock = halfmove_clock * 10 + (fen[i] - '0');
        i++;
    }
    i++;
    fullmove_number = 0;
    while (fen[i] >= '0' && fen[i] <= '9') {
        fullmove_number = fullmove_number * 10 + (fen[i] - '0');
        i++;
    }

}

void Game::make_move(Move move) {
    
}

std::string Game::to_string() {
    Bitboard pos = 0x8000000000000000;
    int i = 0;
    std::string s = "";

    while(pos){
        if(i % 8 == 0 && i != 0){
            s.append("\n");
        }

        if(pos & this->pieces[WHITE][PAWN]){
            s.append("P");
        } else if(pos & this->pieces[WHITE][KNIGHT]){
            s.append("N");
        } else if(pos & this->pieces[WHITE][BISHOP]){
            s.append("B");
        } else if(pos & this->pieces[WHITE][ROOK]){
            s.append("R");
        } else if(pos & this->pieces[WHITE][QUEEN]){
            s.append("Q");
        } else if(pos & this->pieces[WHITE][KING]){
            s.append("K");
        } else if(pos & this->pieces[BLACK][PAWN]){
            s.append("p");
        } else if(pos & this->pieces[BLACK][KNIGHT]){
            s.append("n");
        } else if(pos & this->pieces[BLACK][BISHOP]){
            s.append("b");
        } else if(pos & this->pieces[BLACK][ROOK]){
            s.append("r");
        } else if(pos & this->pieces[BLACK][QUEEN]){
            s.append("q");
        } else if(pos & this->pieces[BLACK][KING]){
            s.append("k");
        } else {
            s.append("-");
        }

        pos = pos >> 1;
        i++;

        
    }

    return s;
}

std::string Move::to_string() {
    std::string move_string = "";
    
    //from square
    /*
    if(this->from & FILE_A){
        move_string.append("a");
    } else if(this->from & FILE_B){
        move_string.append("b");
    } else if(this->from & FILE_C){
        move_string.append("c");
    } else if(this->from & FILE_D){
        move_string.append("d");
    } else if(this->from & FILE_E){
        move_string.append("e");
    } else if(this->from & FILE_F){
        move_string.append("f");
    } else if(this->from & FILE_G){
        move_string.append("g");
    } else if(this->from & FILE_H){
        move_string.append("h");
    }

    if(this->from & RANK_1){
        move_string.append("1");
    } else if(this->from & RANK_2){
        move_string.append("2");
    } else if(this->from & RANK_3){
        move_string.append("3");
    } else if(this->from & RANK_4){
        move_string.append("4");
    } else if(this->from & RANK_5){
        move_string.append("5");
    } else if(this->from & RANK_6){
        move_string.append("6");
    } else if(this->from & RANK_7){
        move_string.append("7");
    } else if(this->from & RANK_8){
        move_string.append("8");
    }

    move_string += "-";
    */

    if(this->piece_type == KNIGHT){
        move_string += "N";
    }
    else if(this->piece_type == BISHOP){
        move_string += "B";
    }
    else if(this->piece_type == ROOK){
        move_string += "R";
    }
    else if(this->piece_type == QUEEN){
        move_string += "Q";
    }
    else if(this->piece_type == KING){
        move_string += "K";
    }

    if(this->capture){
        move_string += "x";
    }

    if(this->to & FILE_A){
        move_string += "a";
    }
    else if(this->to & FILE_B){
        move_string += "b";
    }
    else if(this->to & FILE_C){
        move_string += "c";
    }
    else if(this->to & FILE_D){
        move_string += "d";
    }
    else if(this->to & FILE_E){
        move_string += "e";
    }
    else if(this->to & FILE_F){
        move_string += "f";
    }
    else if(this->to & FILE_G){
        move_string += "g";
    }
    else if(this->to & FILE_H){
        move_string += "h";
    }

    if(this->to & RANK_1){
        move_string += "1";
    }
    else if(this->to & RANK_2){
        move_string += "2";
    }
    else if(this->to & RANK_3){
        move_string += "3";
    }
    else if(this->to & RANK_4){
        move_string += "4";
    }
    else if(this->to & RANK_5){
        move_string += "5";
    }
    else if(this->to & RANK_6){
        move_string += "6";
    }
    else if(this->to & RANK_7){
        move_string += "7";
    }
    else if(this->to & RANK_8){
        move_string += "8";
    }

    if(this->is_promotion){
        move_string += "=";
        if(this->promotion == KNIGHT){
            move_string += "N";
        }
        else if(this->promotion == BISHOP){
            move_string += "B";
        }
        else if(this->promotion == ROOK){
            move_string += "R";
        }
        else if(this->promotion == QUEEN){
            move_string += "Q";
        }
    }

    if(this->check){
        move_string += "+";
    }

    return move_string;
}

