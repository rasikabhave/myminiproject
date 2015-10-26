#include<stdio.h>
#include<ncurses.h>
#include "project.h"
extern int turn;
extern WINDOW *debug,*chessboard;
//pawn
int pawn(int y,int x , int fy, int fx) 
{
	int m=0,killed=0,a=0;
	wrefresh(chessboard);
	if(turn==0)
         {
		if(y==1)
                {
			if((fy==y+2)&&(fx==x)) 
                        {
				if((board[fy][fx]==0) && (board[y+1][x]==0))
				{
					mvwprintw(debug,10,10,"true1");wrefresh(debug);			
					a=1;
				}
			}
			else if((fy==y+1)&&(fx==x)) 
		{
 			mvwprintw(debug,10,10,"true2");wrefresh(debug);;
			if(board[fy][fx]==0)
			{
				a=1;//mvwprintw(debug,25,80,"iftwo");wrefresh(chessboard);;
			}
		}
			else if(fy>y+2)
				a=0;
		}
		else if((fy==y+1)&&(fx==x)) 
		{
 			mvwprintw(debug,10,10,"true2");wrefresh(debug);;
			if(board[fy][fx]==0)
			{
				a=1;//mvwprintw(debug,25,80,"iftwo");wrefresh(chessboard);;
			}
		}
		else if((fy==y+1)&&((fx==x+1)||(fx==x-1)))
		{
			mvwprintw(debug,10,10,"true3");wrefresh(debug);;
			if(board[fy][fx]<0)
			{
				a=1;//mvwprintw(debug,25,80,"wifthree");wrefresh(chessboard);;
				killed=1;
				m=board[fy][fx];
			}
		}	
		if (a==0) 
		{
			mvwprintw(debug,10,10,"invalid");wrefresh(debug);
		}		//wrefresh(chessboard);;
		else if(a==1)
		{            	mvwprintw(debug,10,10,"a is very good");wrefresh(debug);;
				//movepiece(y,x,fy,fx);wrefresh(chessboard);;
				if(killed==1)
				{
//					mvwprintw(debug,10,10,"%d",m);
				}
		
		//promote condition check where pawn is
				if(fy==7)
				{
					//promotepawn(fx,fy)
				}
		}
		
	}
	else if(turn!=0) 
	{
		if(y==6)
		{
			if((fy==(y-2)) && (fx==x))
			{
				if((board[fy][fx]==0)&&(board[y-1][x]==0))
				{
					a = 1;
				}
			}
		
		}
		else if((fy==y-1)&&(fx==x))	
		{
			if(board[fy][fx]==0)
			{
				a = 1;
			}
		}
		else if((fy==y-1) && ((fx==x-1)||(fx==x+1)))
		{
			if(board[fy][fx]>0)
			{
				a = 1;
				killed=1;
				m=board[fy][fx];
			}
		}
		else 
		{	mvwprintw(debug,10,10,"b is very good");wrefresh(debug);;
			a=0;
			mvwprintw(debug,10,10,"invalid");return;
			wrefresh(chessboard);;
		}
		//if valid move i.e. a=1	
		if(a==1){
			mvwprintw(debug,10,10,"because of player 1");wrefresh(debug);;
			//movepiece(y,x,fy,fx);
			wrefresh(chessboard);;
			if(killed==1){
				//add m to a list of taken pieces
			}
			if(fy==0){
			//promotepawn(fx,fy)
			}
		}
	}/*if(turn==0) 
			turn = 1;
		else 
			turn = 0;*/
	return a;
}
//en-passant: when a pawn makes a double step from second row to the fourth and there is an enemy pawn on an adjacent square on the fourth row then this enemy pawn in the next move may move diagonally to the square that was passed over by the double stepping pawn. this must be done directly: if the player who could en passant does not do this in the first move after the double step, this pawn cannot be taken anymore by an en passant
// to promote a pawn
void promotepawn(int x,int y){
	//what would u like to promote it to and get the input
	board[y][x]=0;//whatever the user wants to promote it to
}
//horse
int horse(int x,int y,int x1,int y1){
	int a=0,m=0;
	if((y1==y+2 && x1==x+1)||(y1==y+2 && x1==x-1)||(y1==y+1 && x1==x+2)||(y1==y+1 && x1==x-2)||(y1==y-1 && x1==x+2)||(y1==y-1 && x1==x-2)||(y1==y-2 && x1==x+1)||(y1==y-2 && x1==x-1)){
		a = 1;
	}
	if(turn==0){
		if(board[x1][y1]<0){
			killed=1;
			m=board[x1][y1];
		}
		if(board[x1][y1]>0)
			a=0;
	}
	else if(turn!=0){
		if(board[x1][y1]>0){
			killed=1;
			m=board[x1][y1];
		}
		if(board[x1][y1]<0)
			a=0;
	}
	if(a==1)
		movepiece(y,x,y1,x1);
	return a;
}

void movepiece(int y,int x, int fy,int fx) {
mvwprintw(debug,1,1,"%d%d",x,y);wrefresh(debug);;
	int m,n;
		m=board[fx][fy];
			n=board[x][y];
			board[fy][fx]=board[y][x];
			board[y][x]=0;
	if((x+y)%2==0){
		mvwaddch(chessboard,STRTBY+(3*y +2),STRTBX+(6*x +3), ACS_CKBOARD);
		mvwaddch(chessboard,STRTBY+(3*y +2),STRTBX+(6*x +4), ACS_CKBOARD);
	}
	else{
		mvwaddch(chessboard,STRTBY+(3*y +2),STRTBX+(6*x +3),' ');
		mvwaddch(chessboard,STRTBY+(3*y +2),STRTBX+(6*x +4),' ');
	}	switch(board[fy][fx]){
		case 9:case 10:case 11:case 12:case 13:case 14:case 15:case 16:
				mvwprintw(chessboard,STRTBY+(3*fy +2), STRTBX+(6*fx +3),"P2");
									break;
				case 2: case 7:mvwprintw(chessboard,STRTBY+(3*fy +2), STRTBX+(6*fx +3),"H2");
								break;
				case 4: mvwprintw(chessboard,STRTBY+(3*fy +2), STRTBX+(6*fx +3),"Q2");
								break;
				case 3:case 6:mvwprintw(chessboard,STRTBY+(3*fy +2), STRTBX+(6*fx +3),"B2");
								break;
				case 1:case 8:mvwprintw(chessboard,STRTBY+(3*fy +2), STRTBX+(6*fx +3),"R2");
								break;
				case 5:mvwprintw(chessboard,STRTBY+(3*fy +2), STRTBX+(6*fx +3),"K2");
								break;
				case -9:case -10:case -11:case -12:case -13:case -14:case -15:case -16:
				mvwprintw(chessboard,STRTBY+(3*fy +2), STRTBX+(6*fx +3),"P1");
									break;
				case -2: case -7:mvwprintw(chessboard,STRTBY+(3*fy +2), STRTBX+(6*fx +3),"H1");
								break;
				case -4: mvwprintw(chessboard,STRTBY+(3*fy +2), STRTBX+(6*fx +3),"Q1");
								break;
				case -3:case -6:mvwprintw(chessboard,STRTBY+(3*fy +2), STRTBX+(6*fx +3),"B1");
								break;
				case -1:case -8:mvwprintw(chessboard,STRTBY+(3*fy +2), STRTBX+(6*fx +3),"R1");
								break;
				case -5:mvwprintw(chessboard,STRTBY+(3*fy +2), STRTBX+(6*fx +3),"K1");
						break;
	}
	mvwprintw(debug,5,19,"     %d         ",board[fy][fx]);wrefresh(debug);;
			
}
//king
int king(int x,int y,int x1,int y1){
	int a = 0;	
		if((y1==y+1 && x1==x)||(y1==y-1 && x1==x)||(y1==y+1 && x1==x+1)||(y1==y+1 && x1==x-1)||(y1==y-1 && x1==x+1)||(y1==y-1 && x1==x-1)||(y1==y && x1==x+1)||(y1==y && x1==x-1)){
			a = 1;
		}
		//castling
		else if(board[y][x+1]==0&&board[y][x+2]==0&&(abs(board[y1][x1]) ==abs(board[y][x+3])==1||abs(board[y1][x1])== abs(board[y][x+3])==8)){
		return 2;
		} 
		if(a==1){
			if(board[y][x]==5) 
				count1=1; 
			if(board[y][x]==-5)
				count2=1;
		}
		return a; 
}
//bishop
int bishop(int y,int x,int y1,int x1) {
	int a=1,i;mvwprintw(debug,25,80,"working");mvwprintw(debug,3,3,"%d %d %d %d",y,x,y1,x1);wrefresh(debug);;
	if(abs(x1-x)!=abs(y1-y)) {
		a=0;
	}
	if((x<x1)&&(y<y1)) { 
		for(i=1;(i+x)<x1;i++) {
			if(board[y+i][x+i]!=0) 
				a=0;
		}
	}
	else if((x>x1)&&(y>y1)){ 
		for(i=1;(x-i)>x1;i++){
			if(board[y-i][x-i]!=0)
				a=0;
		}
	}
	else if((x>x1)&&(y<y1)){
		for(i=1;(x-i)>x1;i++){ 
			if(board[y+i][x-i]!=0) 
				a=0;
		} 
	}
	else if((x<x1)&&(y>y1)){
		for(i=1;(y-i)>y1;i++){
			if(board[y-i][x+i]!=0)
				a=0; 
		}	
	}
	if((turn==0)&&board[y1][x1]>0)
		a=0;
		return a;
}
//queen
int queen(int x,int y,int x1,int y1){
	int a=0;
	if(x==x1||y==y1){
		//if queen moves in + direction
	a=1;	return rook(x,y,x1,y1);
	}
	else if(abs(x1-x)==abs(y-y1)){
		//if queen moves in diagnol direction
	a=1; return bishop(x,y,x1,y1); 
	}
	return a;
} 
//rook
int rook(int x,int y,int x1,int y1){ 
	int i,a=1; 
	if(y1==y) {
		for(i=x+1;i<x1;i++) { 
			if(board[y1][i]!=0) { 
				a = 0;
				break;
			}
		}
		for(i=x-1;i>x1;i--) { 
			if(board[y1][i]!=0) {
				a = 0;
				break;
			}
		}
	}
	else if(x1==x) { 
		for(i=y+1;i<y1;i++) {
			if(board[i][x1]!=0) {
				a = 0;
				break;
			}
		}
		for(i=y-1;i>y1;i--) {
			if(board[i][x1]!=0) {
				a = 0;
				break;
			}
		}
	}
	else {
		a=0;
	}
	return a;
}
