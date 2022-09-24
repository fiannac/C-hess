#define SDL_MAIN_HANDLED

#include <iostream>
#include <fstream>
#include <stdlib.h>
using namespace std;

int depth_max = 3;
int value_matrix[12][8][8];
int chessboard_values[8][8];
int checks = 0;

typedef struct {
	char id;
	bool player;
	bool unmoved;
}  piece_t;

typedef struct{
	piece_t to_move;
	int x;
	int y;
	int oldx;
	int oldy;
	int flag = 0;
	bool terminator = false;
	float value;
} move_t;

typedef struct{
	piece_t board[8][8];
	int w_king_x;
	int w_king_y;
	int b_king_x;
	int b_king_y;
	bool w_castled = false;
	bool b_castled = false;
} chessboard_t;

void make_move(move_t mov,chessboard_t& chessboard){
	
	chessboard.board[mov.x][mov.y] = mov.to_move;
	chessboard.board[mov.x][mov.y].unmoved = false;
	chessboard.board[mov.oldx][mov.oldy].id = 0;

	if(mov.flag == 1){ // arrocco corto
		chessboard.board[5][mov.y] = chessboard.board[7][mov.y];
		chessboard.board[7][mov.y].id = 0;
		if(chessboard.board[mov.x][mov.y].id == true)
			chessboard.w_castled = true;
		else chessboard.b_castled = true;
	}
	if(mov.flag == 2){ // arrocco corto
		chessboard.board[3][mov.y] = chessboard.board[0][mov.y];
		chessboard.board[0][mov.y].id = 0;
		if(chessboard.board[mov.x][mov.y].id == true)
			chessboard.w_castled = true;
		else chessboard.b_castled = true;
	}

	if(mov.to_move.id == 'K'){
		if(mov.to_move.player == true){
			chessboard.w_king_x = mov.x;
			chessboard.w_king_y = mov.y;
		} else {
			chessboard.b_king_x = mov.x;
			chessboard.b_king_y = mov.y;
		}
	}
	if(mov.to_move.id == 'p')
		if(mov.y == 0 || mov.y == 7) chessboard.board[mov.x][mov.y].id = 'q'; 

	return;
}

bool in_board(int x,int y){
	if(x > 7 || x < 0 || y > 7 || y < 0) return false;
	return true;
}

void copy_chessboard(chessboard_t from,chessboard_t& to){
	for(int r = 0; r<8 ; r++)
		for(int c = 0; c<8; c++)
			to.board[c][r] = from.board[c][r];

	to.w_king_x = from.w_king_x;
	to.b_king_x = from.b_king_x;
	to.w_king_y = from.w_king_y;
	to.b_king_y = from.b_king_y;
	return;
}

bool scacco(chessboard_t& chessboard, bool player){
	int x;
	int y;
	int k;

	if(player == true){
		x = chessboard.w_king_x;
		y = chessboard.w_king_y;
		k = 1;
	} else {
		x = chessboard.b_king_x;
		y = chessboard.b_king_y;
		k = -1;
	}

	//pawn
	if(in_board(x+1,y-k) == true)
		if(chessboard.board[x+1][y-k].id == 'p' && chessboard.board[x+1][y-k].player != player)
			return true;
	
	if(in_board(x-1,y-k) == true)
		if(chessboard.board[x-1][y-k].id == 'p' && chessboard.board[x-1][y-k].player != player)
			return true;
	

	//knight
	if(in_board(x+2,y+1))
		if(chessboard.board[x+2][y+1].player != player && chessboard.board[x+2][y+1].id == 'k')
			return true;
	
	if(in_board(x-2,y+1))
		if(chessboard.board[x-2][y+1].player != player && chessboard.board[x-2][y+1].id == 'k')
			return true;
		
	if(in_board(x+2,y-1))
		if(chessboard.board[x+2][y-1].player != player && chessboard.board[x+2][y-1].id == 'k')
			return true;
		
	if(in_board(x-2,y-1))
		if(chessboard.board[x-2][y-1].player != player && chessboard.board[x-2][y-1].id == 'k')
			return true;
		
	
	if(in_board(x+1,y+2))
		if(chessboard.board[x+1][y+2].player != player && chessboard.board[x+1][y+2].id == 'k')
			return true;
	
	if(in_board(x-1,y+2))
		if(chessboard.board[x-1][y+2].player != player && chessboard.board[x-1][y+2].id == 'k')
			return true;
	
	if(in_board(x+1,y-2))
		if(chessboard.board[x+1][y-2].player != player && chessboard.board[x+1][y-2].id == 'k')
			return true;

	if(in_board(x-1,y-2))
		if(chessboard.board[x-1][y-2].player != player && chessboard.board[x-1][y-2].id == 'k')
			return true;

	//rook
	int temp_x,temp_y;
	bool checked;
	for(temp_x=x+1,temp_y=y,checked = false; checked == false; temp_x++){
		if(in_board(temp_x,temp_y)==true){
			if(chessboard.board[temp_x][temp_y].id != 0){
				if(chessboard.board[temp_x][temp_y].player != player){
					if(chessboard.board[temp_x][temp_y].id == 'r' || chessboard.board[temp_x][temp_y].id == 'q') return true;
					else checked = true;
				} else checked = true;
			}
		}		
		else checked = true;
	}

	for(temp_x=x-1,temp_y=y,checked = false; checked == false; temp_x--){
		if(in_board(temp_x,temp_y)==true){
			if(chessboard.board[temp_x][temp_y].id != 0){
				if(chessboard.board[temp_x][temp_y].player != player){
					if(chessboard.board[temp_x][temp_y].id == 'r' || chessboard.board[temp_x][temp_y].id == 'q') return true;
					else checked = true;
				} else checked = true;
			}
		}		
		else checked = true;
	}

	for(temp_x=x,temp_y=y+1,checked = false; checked == false; temp_y++){
		if(in_board(temp_x,temp_y)==true){
			if(chessboard.board[temp_x][temp_y].id != 0){
				if(chessboard.board[temp_x][temp_y].player != player){
					if(chessboard.board[temp_x][temp_y].id == 'r' || chessboard.board[temp_x][temp_y].id == 'q') return true;
					else checked = true;
				} else checked = true;
			}
		}		
		else checked = true;
	}


	for(temp_x=x,temp_y=y-1,checked = false; checked == false; temp_y--){
		if(in_board(temp_x,temp_y)==true){
			if(chessboard.board[temp_x][temp_y].id != 0){
				if(chessboard.board[temp_x][temp_y].player != player){
					if(chessboard.board[temp_x][temp_y].id == 'r' || chessboard.board[temp_x][temp_y].id == 'q') return true;
					else checked = true;
				} else checked = true;
			}
		}		
		else checked = true;
	}

	// bishop 

	for(temp_x=x+1,temp_y=y+1,checked = false; checked == false; temp_x++,temp_y++){
		if(in_board(temp_x,temp_y)==true){
			if(chessboard.board[temp_x][temp_y].id != 0){
				if(chessboard.board[temp_x][temp_y].player != player){
					if(chessboard.board[temp_x][temp_y].id == 'b' || chessboard.board[temp_x][temp_y].id == 'q') return true;
					else checked = true;
				} else checked = true;
			}
		}		
		else checked = true;
	}

	for(temp_x=x-1,temp_y=y-1,checked = false; checked == false; temp_x--,temp_y--){
		if(in_board(temp_x,temp_y)==true){
			if(chessboard.board[temp_x][temp_y].id != 0){
				if(chessboard.board[temp_x][temp_y].player != player){
					if(chessboard.board[temp_x][temp_y].id == 'b' || chessboard.board[temp_x][temp_y].id == 'q') return true;
					else checked == true;
				} else checked = true;
			}
		}		
		else checked = true;
	}

	for(temp_x=x-1,temp_y=y+1,checked = false; checked == false; temp_y++,temp_x--){
		if(in_board(temp_x,temp_y)==true){
			if(chessboard.board[temp_x][temp_y].id != 0){
				if(chessboard.board[temp_x][temp_y].player != player){
					if(chessboard.board[temp_x][temp_y].id == 'b' || chessboard.board[temp_x][temp_y].id == 'q') return true;
					else checked = true;
				} else checked = true;
			}
		}		
		else checked = true;
	}


	for(temp_x=x+1,temp_y=y-1,checked = false; checked == false; temp_y--,temp_x++){
		if(in_board(temp_x,temp_y)==true){
			if(chessboard.board[temp_x][temp_y].id != 0){
				if(chessboard.board[temp_x][temp_y].player != player){
					if(chessboard.board[temp_x][temp_y].id == 'b' || chessboard.board[temp_x][temp_y].id == 'q') return true;
					else checked = true;
				} else checked = true;
			}
		}		
		else checked = true;
	}

	return false;
}

void addmove(int oldx,int oldy,int x, int y, move_t moves[],int& i,piece_t piece,int flag,chessboard_t& chessboard, bool pseudolegal){
	moves[i].terminator = false;
	moves[i].x = x;
	moves[i].y = y;
	moves[i].oldx = oldx;
	moves[i].oldy = oldy;
	moves[i].to_move = piece;
	moves[i].flag = flag;
	if(pseudolegal == false){
		chessboard_t temp;
		copy_chessboard(chessboard,temp);
		make_move(moves[i],temp);
		if(scacco(temp,piece.player)) return;
	}
	i++;
	return;
}

void pawn(piece_t pawn,chessboard_t& chessboard,move_t moves[],bool player,int &i,int x,int y, bool pseudolegal){

	int k=1;
	if(player == false) k=-1;

	if(in_board(x,y-1))
		if(chessboard.board[x][y-k].id == 0){
			addmove(x,y,x,y-k,moves,i,pawn,0,chessboard,pseudolegal);
			if(pawn.unmoved == true)
				if(chessboard.board[x][y-2*k].id == 0)
					addmove(x,y,x,y-2*k,moves,i,pawn,0,chessboard,pseudolegal);			
		}
	
	if(in_board(x-1,y-k))
		if(chessboard.board[x-1][y-k].player != player && chessboard.board[x-1][y-k].id !=0)
			addmove(x,y,x-1,y-k,moves,i,pawn,0,chessboard,pseudolegal);

	if(in_board(x+1,y-k))
		if(chessboard.board[x+1][y-k].player != player && chessboard.board[x+1][y-k].id !=0)
			addmove(x,y,x+1,y-k,moves,i,pawn,0,chessboard,pseudolegal);
}

void knight(piece_t knight,chessboard_t& chessboard,move_t moves[],bool player,int &i,int x,int y, bool pseudolegal){

	if(in_board(x+2,y+1))
		if(chessboard.board[x+2][y+1].player != knight.player || chessboard.board[x+2][y+1].id == 0)
			addmove(x,y,x+2,y+1,moves,i,knight,0,chessboard,pseudolegal);

	if(in_board(x-2,y+1))
		if(chessboard.board[x-2][y+1].player != knight.player || chessboard.board[x-2][y+1].id == 0)
			addmove(x,y,x-2,y+1,moves,i,knight,0,chessboard,pseudolegal);
		
	if(in_board(x+2,y-1))
		if(chessboard.board[x+2][y-1].player != knight.player || chessboard.board[x+2][y-1].id == 0)
			addmove(x,y,x+2,y-1,moves,i,knight,0,chessboard,pseudolegal);
		
	if(in_board(x-2,y-1))
		if(chessboard.board[x-2][y-1].player != knight.player || chessboard.board[x-2][y-1].id == 0)
			addmove(x,y,x-2,y-1,moves,i,knight,0,chessboard,pseudolegal);
		
	if(in_board(x+1,y+2))
		if(chessboard.board[x+1][y+2].player != knight.player || chessboard.board[x+1][y+2].id == 0)
			addmove(x,y,x+1,y+2,moves,i,knight,0,chessboard,pseudolegal);
	
	if(in_board(x-1,y+2))
		if(chessboard.board[x-1][y+2].player != knight.player || chessboard.board[x-1][y+2].id == 0)
			addmove(x,y,x-1,y+2,moves,i,knight,0,chessboard,pseudolegal);
	
	if(in_board(x+1,y-2))
		if(chessboard.board[x+1][y-2].player != knight.player || chessboard.board[x+1][y-2].id == 0)
			addmove(x,y,x+1,y-2,moves,i,knight,0,chessboard,pseudolegal);
	
	if(in_board(x-1,y-2))
		if(chessboard.board[x-1][y-2].player != knight.player || chessboard.board[x-1][y-2].id == 0)
			addmove(x,y,x-1,y-2,moves,i,knight,0,chessboard,pseudolegal);
}

void rook(piece_t rook,chessboard_t& chessboard,move_t moves[],bool player,int &i,int x,int y, bool pseudolegal){

	int temp_x,temp_y;
	bool checked;

	for(temp_x=x+1,temp_y=y,checked = false; checked == false; temp_x++){
		if(in_board(temp_x,temp_y)){
			if(chessboard.board[temp_x][temp_y].id != 0){
				if(chessboard.board[temp_x][temp_y].player == player){
					checked = true;
				} else {
					addmove(x,y,temp_x,temp_y,moves,i,rook,0,chessboard,pseudolegal);
					checked = true;
				}
			} else {
				addmove(x,y,temp_x,temp_y,moves,i,rook,0,chessboard,pseudolegal);
			}
		} else {
			checked = true;
		}
	}

	for(temp_x=x-1,temp_y=y,checked = false; checked == false; temp_x--){
		if(in_board(temp_x,temp_y)){
			if(chessboard.board[temp_x][temp_y].id != 0){
				if(chessboard.board[temp_x][temp_y].player == player){
					checked = true;
				} else {
					addmove(x,y,temp_x,temp_y,moves,i,rook,0,chessboard,pseudolegal);
					checked = true;
				}
			} else {
				addmove(x,y,temp_x,temp_y,moves,i,rook,0,chessboard,pseudolegal);
			}
		} else {
			checked = true;
		}
	}

	for(temp_x=x,temp_y=y+1,checked = false; checked == false; temp_y++){
		if(in_board(temp_x,temp_y)){
			if(chessboard.board[temp_x][temp_y].id != 0){
				if(chessboard.board[temp_x][temp_y].player == player){
					checked = true;
				} else {
					addmove(x,y,temp_x,temp_y,moves,i,rook,0,chessboard,pseudolegal);
					checked = true;
				}
			} else {
				addmove(x,y,temp_x,temp_y,moves,i,rook,0,chessboard,pseudolegal);
			}
		} else {
			checked = true;
		}
	}

	for(temp_x=x,temp_y=y-1,checked = false; checked == false; temp_y--){
		if(in_board(temp_x,temp_y)){
			if(chessboard.board[temp_x][temp_y].id != 0){
				if(chessboard.board[temp_x][temp_y].player == player){
					checked = true;
				} else {
					addmove(x,y,temp_x,temp_y,moves,i,rook,0,chessboard,pseudolegal);
					checked = true;
				}
			} else {
				addmove(x,y,temp_x,temp_y,moves,i,rook,0,chessboard,pseudolegal);
			}
		} else {
			checked = true;
		}
	}
}

void bishop(piece_t bishop,chessboard_t& chessboard,move_t moves[],bool player,int &i,int x,int y, bool pseudolegal){

	int temp_x,temp_y;
	bool checked;
	for(temp_x=x+1,temp_y=y+1,checked = false; checked == false; temp_x++,temp_y++){
		if(in_board(temp_x,temp_y)){
			if(chessboard.board[temp_x][temp_y].id != 0){
				if(chessboard.board[temp_x][temp_y].player == player){
					checked = true;
				} else {
					addmove(x,y,temp_x,temp_y,moves,i,bishop,0,chessboard,pseudolegal);
					checked = true;
				}
			} else {
				addmove(x,y,temp_x,temp_y,moves,i,bishop,0,chessboard,pseudolegal);
			}
		} else {
			checked = true;
		}
	}

	for(temp_x=x-1,temp_y=y-1,checked = false; checked == false; temp_x--,temp_y--){
		if(in_board(temp_x,temp_y)){
			if(chessboard.board[temp_x][temp_y].id != 0){
				if(chessboard.board[temp_x][temp_y].player == player){
					checked = true;
				} else {
					addmove(x,y,temp_x,temp_y,moves,i,bishop,0,chessboard,pseudolegal);
					checked = true;
				}
			} else {
				addmove(x,y,temp_x,temp_y,moves,i,bishop,0,chessboard,pseudolegal);
			}
		} else {
			checked = true;
		}
	}

	for(temp_x=x-1,temp_y=y+1,checked = false; checked == false; temp_y++,temp_x--){
		if(in_board(temp_x,temp_y)){
			if(chessboard.board[temp_x][temp_y].id != 0){
				if(chessboard.board[temp_x][temp_y].player == player){
					checked = true;
				} else {
					addmove(x,y,temp_x,temp_y,moves,i,bishop,0,chessboard,pseudolegal);
					checked = true;
				}
			} else {
				addmove(x,y,temp_x,temp_y,moves,i,bishop,0,chessboard,pseudolegal);
			}
		} else {
			checked = true;
		}
	}

	for(temp_x=x+1,temp_y=y-1,checked = false; checked == false; temp_y--,temp_x++){
		if(in_board(temp_x,temp_y)){
			if(chessboard.board[temp_x][temp_y].id != 0){
				if(chessboard.board[temp_x][temp_y].player == player){
					checked = true;
				} else {
					addmove(x,y,temp_x,temp_y,moves,i,bishop,0,chessboard,pseudolegal);
					checked = true;
				}
			} else {
				addmove(x,y,temp_x,temp_y,moves,i,bishop,0,chessboard,pseudolegal);
			}
		} else {
			checked = true;
		}
	}
}

void queen(piece_t queen,chessboard_t& chessboard,move_t moves[],bool player,int &i,int x,int y, bool pseudolegal){

	int temp_x,temp_y;
	bool checked;

	for(temp_x=x+1,temp_y=y,checked = false; checked == false; temp_x++){
		if(in_board(temp_x,temp_y)){
			if(chessboard.board[temp_x][temp_y].id != 0){
				if(chessboard.board[temp_x][temp_y].player == player){
					checked = true;
				} else {
					addmove(x,y,temp_x,temp_y,moves,i,queen,0,chessboard,pseudolegal);
					checked = true;
				}
			} else {
				addmove(x,y,temp_x,temp_y,moves,i,queen,0,chessboard,pseudolegal);
			}
		} else {
			checked = true;
		}
	}

	for(temp_x=x-1,temp_y=y,checked = false; checked == false; temp_x--){
		if(in_board(temp_x,temp_y)){
			if(chessboard.board[temp_x][temp_y].id != 0){
				if(chessboard.board[temp_x][temp_y].player == player){
					checked = true;
				} else {
					addmove(x,y,temp_x,temp_y,moves,i,queen,0,chessboard,pseudolegal);
					checked = true;
				}
			} else {
				addmove(x,y,temp_x,temp_y,moves,i,queen,0,chessboard,pseudolegal);
			}
		} else {
			checked = true;
		}
	}

	for(temp_x=x,temp_y=y+1,checked = false; checked == false; temp_y++){
		if(in_board(temp_x,temp_y)){
			if(chessboard.board[temp_x][temp_y].id != 0){
				if(chessboard.board[temp_x][temp_y].player == player){
					checked = true;
				} else {
					addmove(x,y,temp_x,temp_y,moves,i,queen,0,chessboard,pseudolegal);
					checked = true;
				}
			} else {
				addmove(x,y,temp_x,temp_y,moves,i,queen,0,chessboard,pseudolegal);
			}
		} else {
			checked = true;
		}
	}

	for(temp_x=x,temp_y=y-1,checked = false; checked == false; temp_y--){
		if(in_board(temp_x,temp_y)==true){
			if(chessboard.board[temp_x][temp_y].id != 0){
				if(chessboard.board[temp_x][temp_y].player == player){
					checked = true;
				} else {
					addmove(x,y,temp_x,temp_y,moves,i,queen,0,chessboard,pseudolegal);
					checked = true;
				}
			} else {
				addmove(x,y,temp_x,temp_y,moves,i,queen,0,chessboard,pseudolegal);
			}
		} else {
			checked = true;
		}
	}

	for(temp_x=x+1,temp_y=y+1,checked = false; checked == false; temp_x++,temp_y++){
		if(in_board(temp_x,temp_y)){
			if(chessboard.board[temp_x][temp_y].id != 0){
				if(chessboard.board[temp_x][temp_y].player == player){
					checked = true;
				} else {
					addmove(x,y,temp_x,temp_y,moves,i,queen,0,chessboard,pseudolegal);
					checked = true;
				}
			} else {
				addmove(x,y,temp_x,temp_y,moves,i,queen,0,chessboard,pseudolegal);
			}
		} else {
			checked = true;
		}
	}

	for(temp_x=x-1,temp_y=y-1,checked = false; checked == false; temp_x--,temp_y--){
		if(in_board(temp_x,temp_y)){
			if(chessboard.board[temp_x][temp_y].id != 0){
				if(chessboard.board[temp_x][temp_y].player == player){
					checked = true;
				} else {
					addmove(x,y,temp_x,temp_y,moves,i,queen,0,chessboard,pseudolegal);
					checked = true;
				}
			} else {
				addmove(x,y,temp_x,temp_y,moves,i,queen,0,chessboard,pseudolegal);
			}
		} else {
			checked = true;
		}
	}

	for(temp_x=x-1,temp_y=y+1,checked = false; checked == false; temp_y++,temp_x--){
		if(in_board(temp_x,temp_y)){
			if(chessboard.board[temp_x][temp_y].id != 0){
				if(chessboard.board[temp_x][temp_y].player == player){
					checked = true;
				} else {
					addmove(x,y,temp_x,temp_y,moves,i,queen,0,chessboard,pseudolegal);
					checked = true;
				}
			} else {
				addmove(x,y,temp_x,temp_y,moves,i,queen,0,chessboard,pseudolegal);
			}
		} else {
			checked = true;
		}
	}

	for(temp_x=x+1,temp_y=y-1,checked = false; checked == false; temp_y--,temp_x++){
		if(in_board(temp_x,temp_y)){
			if(chessboard.board[temp_x][temp_y].id != 0){
				if(chessboard.board[temp_x][temp_y].player == player){
					checked = true;
				} else {
					addmove(x,y,temp_x,temp_y,moves,i,queen,0,chessboard,pseudolegal);
					checked = true;
				}
			} else {
				addmove(x,y,temp_x,temp_y,moves,i,queen,0,chessboard,pseudolegal);
			}
		} else {
			checked = true;
		}
	}
}

void king(piece_t king,chessboard_t& chessboard,move_t moves[],bool player,int &i,int x,int y, bool pseudolegal){

	if(in_board(x+1,y+1)){
		if(chessboard.board[x+1][y+1].id == 0 || chessboard.board[x+1][y+1].player != player){
			addmove(x,y,x+1,y+1,moves,i,king,0,chessboard,pseudolegal);	
		}
	}
	if(in_board(x-1,y+1)){
		if(chessboard.board[x-1][y+1].id == 0 || chessboard.board[x-1][y+1].player != player){
			addmove(x,y,x-1,y+1,moves,i,king,0,chessboard,pseudolegal);
		}
	}
	if(in_board(x+1,y-1)){
		if(chessboard.board[x+1][y-1].id == 0 || chessboard.board[x+1][y-1].player != player){
			addmove(x,y,x+1,y-1,moves,i,king,0,chessboard,pseudolegal);
		}
	}
	if(in_board(x-1,y-1)){
		if(chessboard.board[x-1][y-1].id == 0 || chessboard.board[x-1][y-1].player != player){
			addmove(x,y,x-1,y-1,moves,i,king,0,chessboard,pseudolegal);
		}
	}
	if(in_board(x,y+1)==true){
		if(chessboard.board[x][y+1].id == 0 || chessboard.board[x][y+1].player != player){
			addmove(x,y,x,y+1,moves,i,king,0,chessboard,pseudolegal);
		}
	}
	if(in_board(x,y-1)){
		if(chessboard.board[x][y-1].id == 0 || chessboard.board[x][y-1].player != player){
			addmove(x,y,x,y-1,moves,i,king,0,chessboard,pseudolegal);
		}
	}
	if(in_board(x+1,y)){
		if(chessboard.board[x+1][y].id == 0 || chessboard.board[x+1][y].player != player){
			addmove(x,y,x+1,y,moves,i,king,0,chessboard,pseudolegal);
		}
	}
	if(in_board(x-1,y)){
		if(chessboard.board[x-1][y].id == 0 || chessboard.board[x-1][y].player != player){
			addmove(x,y,x-1,y,moves,i,king,0,chessboard,pseudolegal);
		}
	}

	if(king.unmoved == true){
		if(chessboard.board[x+3][y].id == 'r' && chessboard.board[x+3][y].unmoved == true && chessboard.board[x+1][y].id == 0 && chessboard.board[x+2][y].id == 0){
			addmove(x,y,x+2,y,moves,i,king,1,chessboard,pseudolegal);
		}
		if(chessboard.board[x-4][y].id == 'r' && chessboard.board[x-4][y].unmoved == true && chessboard.board[x-1][y].id == 0 && chessboard.board[x-2][y].id == 0 && chessboard.board[x-3][y].id == 0){
			addmove(x,y,x-2,y,moves,i,king,2,chessboard,pseudolegal);
		}
	}
}

int legal_moves(chessboard_t& chessboard,move_t moves[],bool player,bool pseudolegal){
	int i=0;
	int c,r;
	if(player == true){
		for(c = 0; c<8 ; c++){
			for(r = 7; r>=0; r--){
				if(chessboard.board[r][c].id != 0){
					if(chessboard.board[r][c].player == player){
						if(chessboard.board[r][c].id == 'p') pawn(chessboard.board[r][c],chessboard,moves,player,i,r,c,pseudolegal);
						else if (chessboard.board[r][c].id == 'q') queen(chessboard.board[r][c],chessboard,moves,player,i,r,c,pseudolegal);
						else if (chessboard.board[r][c].id == 'r') rook(chessboard.board[r][c],chessboard,moves,player,i,r,c,pseudolegal);
						else if (chessboard.board[r][c].id == 'k') knight(chessboard.board[r][c],chessboard,moves,player,i,r,c,pseudolegal);
						else if (chessboard.board[r][c].id == 'b') bishop(chessboard.board[r][c],chessboard,moves,player,i,r,c,pseudolegal);
						else if (chessboard.board[r][c].id == 'K') king(chessboard.board[r][c],chessboard,moves,player,i,r,c,pseudolegal);
					}
				}
			}
		}
	}
	else {
		for(c = 7; c>=0 ; c--){
			for(r = 7; r>=0; r--){
				if(chessboard.board[r][c].id != 0){
					if(chessboard.board[r][c].player == player){
						if(chessboard.board[r][c].id == 'p') pawn(chessboard.board[r][c],chessboard,moves,player,i,r,c,pseudolegal);
						else if (chessboard.board[r][c].id == 'q') queen(chessboard.board[r][c],chessboard,moves,player,i,r,c,pseudolegal);
						else if (chessboard.board[r][c].id == 'r') rook(chessboard.board[r][c],chessboard,moves,player,i,r,c,pseudolegal);
						else if (chessboard.board[r][c].id == 'k') knight(chessboard.board[r][c],chessboard,moves,player,i,r,c,pseudolegal);
						else if (chessboard.board[r][c].id == 'b') bishop(chessboard.board[r][c],chessboard,moves,player,i,r,c,pseudolegal);
						else if (chessboard.board[r][c].id == 'K') king(chessboard.board[r][c],chessboard,moves,player,i,r,c,pseudolegal);
					}
				}
			}
		}
	}
	
	moves[i].terminator = true;
	return i;
}

void load_board(chessboard_t& chessboard){
	for(int i = 0 ; i<8 ; i++)
		for(int j=0 ; j<8 ; j++)
			chessboard.board[i][j].id = 0;

	ifstream input;
	input.open("input.txt",ios::in);
	piece_t piece;
	char temp;
	char temp2;
  	while(input.eof()==false){

  		input.get(temp);
  		piece.id = temp;

  		input.get(temp);
  		if(temp == 'w') piece.player = true;
  		else piece.player = false;

  		input.get(temp);
  		if(temp == 'u') piece.unmoved = true;
  		else piece.unmoved = false;

  		input.get(temp);
  		temp = temp -'a';

		input.get(temp2);
  		temp2 = temp2 -'1';

  		chessboard.board[(int)temp][(int)temp2] = piece;

  		input.get(temp);
  		
  	}
  	input.close();

  	chessboard.w_king_x = 4;
  	chessboard.w_king_y = 7;
  	chessboard.b_king_x = 4;
  	chessboard.b_king_y = 0;
}

bool is_legal(move_t& mov, move_t moves[]){
	for(int i=0;moves[i].terminator != true;i++)
		if(mov.x == moves[i].x && mov.y == moves[i].y && mov.oldx == moves[i].oldx && mov.oldy == moves[i].oldy){
			mov.flag = moves[i].flag;
			return true;
		}
	cout << "Mossa non lecita!" << endl;
	return false; 
}

move_t getmove(move_t moves[],chessboard_t chessboard){
	move_t temp;
	int clicks = 0;
	bool finito = false;
	

	return temp;
}

float mult_matrix(int mat1[8][8],int mat2[8][8]){
	float res=0;
	int j;
	for(int i=0;i<8;i++)
		for(j=0;j<8;j++){
			res += mat1[i][j]*mat2[i][j]/100.0;
		}
	return res;
}

int board_control(move_t moves[]){
	int board[8][8]{};
	for(int i=0;moves[i].terminator == false;i++)
		board[moves[i].x][moves[i].y]++;
	return mult_matrix(board,chessboard_values);
}

float value_function(chessboard_t chessboard){
	float value = 0;
	int k;
	int q;
	int m[12][8][8] {};

	for(int i=0;i<8;i++){
		for(int j=0;j<8;j++){
			if(chessboard.board[i][j].id != 0){
				if(chessboard.board[i][j].player == false){
					k=-1;
					q = 1;
				} else{
					k=1;
					q = 0;
				}
				if(chessboard.board[i][j].id == 'p'){
				 	value += 1*k;
				 	m[0+q][i][j] = 1;
				}
				if(chessboard.board[i][j].id == 'r'){
				 	value += 5*k;
				 	m[2+q][i][j] = 1;
				}
				if(chessboard.board[i][j].id == 'b'){
					value += 3.1*k;
					m[4+q][i][j] = 1;
				}
				if(chessboard.board[i][j].id == 'k'){
					value += 3.0*k;
					m[6+q][i][j] = 1;
				}
				if(chessboard.board[i][j].id == 'q'){
					value += 10.0*k;
					m[8+q][i][j] = 1;
				}
				if(chessboard.board[i][j].id == 'K'){
					value += 100000*k;
					m[10+q][i][j] = 1;
				}
			}
		}
	}

	for(int i=0;i<12;i++)
		value += 0.01 * mult_matrix(m[i],value_matrix[i]);


	move_t w_m[200],b_m[200];
	value += 0.001 * (legal_moves(chessboard,w_m,true,false) - legal_moves(chessboard,b_m,false,false));
	value += 0.03 * (board_control(w_m) - board_control(b_m));

	return value;
}

float alpha_beta(move_t mov,chessboard_t chessboard,int depth,float a,float b,bool player){
	make_move(mov,chessboard);
	checks ++;

	move_t moves[200];
	legal_moves(chessboard,moves,player,true);
	float v;
	float t = value_function(chessboard);

	if(depth == 0 || moves[0].terminator == true || t >= 10000 || t <= -10000 )
		return t;

	if(player == true){
		v = -100000;
		for(int i=0,finito = false;moves[i].terminator == false && finito == false;i++){
			moves[i].value = alpha_beta(moves[i],chessboard,depth-1,a,b,false);
			if(moves[i].value > v) v = moves[i].value;
			if(v > a) a = v;
			if(b <= a) finito = true;
		}
		return v;
	}

	if(player == false){
		v = 100000;
		for(int i=0,finito = false;moves[i].terminator == false && finito == false;i++){
			moves[i].value = alpha_beta(moves[i],chessboard,depth-1,a,b,true);
			if(moves[i].value < v) v = moves[i].value;
			if(v < b) b = v;
			if(b <= a) finito = true;
		}
		return v;
	}
}

move_t bot (chessboard_t chessboard,int depth,float a, float b,bool player){
	int j=0;
	move_t moves[200];
	legal_moves(chessboard,moves,player,true);
	float v;
	bool finito;
	

	if(player == true){
		v = -100000;
		for(int i=0,finito = false;moves[i].terminator == false && finito == false;i++){		
			moves[i].value = alpha_beta(moves[i],chessboard,depth-1,a,b,false);
			if(moves[i].value > v){
				v = moves[i].value;
				j = i;
			}
			
			if(v > a) a = v;

			if(b <= a) finito = true;
		}
		return moves[j];
	}

	if(player == false){
		v = 100000;
		for(int i=0,finito = false;moves[i].terminator == false && finito == false;i++){
			moves[i].value = alpha_beta(moves[i],chessboard,depth-1,a,b,true);
			if(moves[i].value < v){
				v = moves[i].value;
				j = i;
			}
			
			if(v < b) b = v;

			if(b <= a) finito = true;
		}
		return moves[j];
	}
}

void load_value_matrix(){
	ifstream input;
	input.open("value_matrix.txt",ios::in);
	int temp;
	char temp2;
	for(int i=0;i < 12 ;i++){
		for(int j = 0;j<8;j++){
			for(int k=0;k<8;k++){
				input >> temp;
				value_matrix[i][k][j] = temp;
			}
			input.get(temp2);
		}
		input.get(temp2);
	}

	input.open("chessboard_matrix.txt",ios::in);

	for(int j = 0;j<8;j++){
		for(int k=0;k<8;k++){
			input >> temp;
			chessboard_values[k][j] = temp;
		}
		input.get(temp2);
	}
}

int main(int argc, char* argv[]){
    return 0;
}