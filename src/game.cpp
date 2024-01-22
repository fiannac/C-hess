#include "game.h"
#include <string>
#include <iostream>
#include <sstream>


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

void Game::make_move(const Move& move) {
    if(turn == WHITE) {
        turn = BLACK;
    } else {
        turn = WHITE;
    }

    PieceType piece_type = move.piece_type;
    Color piece_color = move.color;
    Bitboard from = move.from;
    Bitboard to = move.to;

    

    pieces[piece_color][piece_type] &= ~(from);
    pieces[piece_color][piece_type] |= to;

    occupied[piece_color] &= ~(from);
    occupied[piece_color] |= to;

    all &= ~(from);
    all |= to;

    
    if (move.capture) {
        Color capture_color = (piece_color == WHITE) ? BLACK : WHITE;
        for(int i = 0; i < 6; i++) {
            if(pieces[capture_color][i] & to) {
                pieces[capture_color][i] &= ~(to);
                occupied[capture_color] &= ~(to);
                all &= ~(to);
                break;
            }
        }
    }

    if (move.en_passant){
        if(piece_color == WHITE)
            pieces[BLACK][PAWN] &= ~(to >> 8);
        else if(piece_color == BLACK)
            pieces[WHITE][PAWN] &= ~(to << 8);
    }

    if (move.is_promotion) {
        pieces[piece_color][piece_type] &= ~(to);
        pieces[piece_color][move.promotion] |= to;
    }

    if(move.pawn_double_push){
        if(piece_color == WHITE){
            en_passant = to >> 8;
        } else {
            en_passant = to << 8;
        }
        en_passant_possible = true;
    } else {
        en_passant = 0;
        en_passant_possible = false;
    }

    if(move.castling){
        if(to == 1ULL << G1){
            pieces[WHITE][ROOK] &= ~(1ULL << H1);
            pieces[WHITE][ROOK] |= 1ULL << F1;
        } else if( to == 1ULL << C1){
            pieces[WHITE][ROOK] &= ~(1ULL << A1);
            pieces[WHITE][ROOK] |= 1ULL << D1;
        } else if( to == 1ULL << G8){
            pieces[BLACK][ROOK] &= ~(1ULL << H8);
            pieces[BLACK][ROOK] |= 1ULL << F8;
        } else if( to == 1ULL << C8){
            pieces[BLACK][ROOK] &= ~(1ULL << A8);
            pieces[BLACK][ROOK] |= 1ULL << D8;
        }
    }

    if(move.piece_type == KING){
        if(piece_color == WHITE){
            white_can_castle_kingside = false;
            white_can_castle_queenside = false;
        } else {
            black_can_castle_kingside = false;
            black_can_castle_queenside = false;
        }
    } else if(move.piece_type == ROOK){
        if(piece_color == WHITE){
            if(from == 1ULL << H1){
                white_can_castle_kingside = false;
            } else if(from == 1ULL << A1){
                white_can_castle_queenside = false;
            }
        } else {
            if(from == 1ULL << H8){
                black_can_castle_kingside = false;
            } else if(from == 1ULL << A8){
                black_can_castle_queenside = false;
            }
        }
    }

    if(move.piece_type != PAWN && !move.capture){
        halfmove_clock++;
    } else {
        halfmove_clock = 0;
    }

    fullmove_number ++;

}

std::string Game::to_string() {
    Bitboard pos = 0x8000000000000000;
    std::stringstream board;

    int rank = 8;
    int file = 0;

    board << "+===+===+===+===+===+===+===+===+\n";

    while (pos) {
        if (file % 8 == 0) {
            if (file != 0) {
                board << " " << rank << "\n";
                board << "+---+---+---+---+---+---+---+---+\n";
                rank--;
            }
            board << "║";
        }

        // Append the piece or a dash, followed by a space
        if (pos & this->pieces[WHITE][PAWN]) board << " P ";
        else if (pos & this->pieces[WHITE][KNIGHT]) board << " N ";
        else if (pos & this->pieces[WHITE][BISHOP]) board << " B ";
        else if (pos & this->pieces[WHITE][ROOK]) board << " R ";
        else if (pos & this->pieces[WHITE][QUEEN]) board << " Q ";
        else if (pos & this->pieces[WHITE][KING]) board << " K ";
        else if (pos & this->pieces[BLACK][PAWN]) board << " p ";
        else if (pos & this->pieces[BLACK][KNIGHT]) board << " n ";
        else if (pos & this->pieces[BLACK][BISHOP]) board << " b ";
        else if (pos & this->pieces[BLACK][ROOK]) board << " r ";
        else if (pos & this->pieces[BLACK][QUEEN]) board << " q ";
        else if (pos & this->pieces[BLACK][KING]) board << " k ";
        else board << "   ";

        if((file + 1) % 8) board << "|";
        else board << "║";

        pos >>= 1;
        file++;
    }

    board << " " << rank << "\n";
    board << "+===+===+===+===+===+===+===+===+\n";
    board << "  a   b   c   d   e   f   g   h \n";
    return board.str();
}


std::string Move::to_string() {
    std::string move_string = "";

   if(castling){
    if(to == 1ULL << G1 || to == 1ULL << G8){
        move_string += "O-O";
    } else {
        move_string += "O-O-O";
    }
    return move_string;
   }

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
       // move_string += "+";
    }

    return move_string;
}
