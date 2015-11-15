#include<stdio.h>
#include<ncurses.h>
#include "project.h"
extern int turn;
extern int board[8][8];
extern WINDOW *menuwin,*chessboard,*welcome,*kwin;

extern int *karr;
extern int count;
extern mstack s;
//pawn
int pawn(int y,int x , int fy, int fx) 
{	moves *tmp;
	tmp=mpop(&s);
	int m=0,killed=0,a=0;
	wrefresh(chessboard);
	if(turn==0){
	//enpassant
		if(tmp->srow==6&&tmp->erow==4){
			if(fx==x+1||fx==x-1){
				if(fy==y+1){
					if(fx==tmp->ecol){
						a=1;wprintw(menuwin,"entered enpassant");wrefresh(menuwin);
						karr[count]=board[tmp->erow][tmp->ecol];count++;printelement();
						movepiece(tmp->erow,tmp->ecol,-1,-1);
						return a;
					}
				}
			}
		}
		if(y==1){
			if((fy==y+2)&&(fx==x)){
				if((board[fy][fx]==0) && (board[y+1][x]==0)){
					a=1;
				}
			}
			else if((fy==y+1)&&(fx==x)){
				if(board[fy][fx]==0){
					a=1;		
				}
			}
			else if((fy==y+1)&&((fx==x+1)||(fx==x-1))){
				if(board[fy][fx]<0){
				a=1;		
				}
			}
			else if(fy>y+2)
				a=0;
		}
		else if((fy==y+1)&&(fx==x)){
			if(board[fy][fx]==0){
				a=1;
			}
		}
		else if((fy==y+1)&&((fx==x+1)||(fx==x-1))){
			if(board[fy][fx]<0){
				a=1;
			}
		}	
		if (a==0) {
		}		
		else if(a==1){
			if(fy==7){
				promotepawn(fy,fx);movepiece(y,x,-1,-1);return 0;
			}
		}
	}
	else if(turn!=0) {
		if(tmp->srow==1&&tmp->erow==3){
			if(fx==x+1||fx==x-1){
				if(fy==y-1){
					if(fx==tmp->ecol){
						a=1;karr[count]=board[tmp->erow][tmp->ecol];count++;printelement();
						movepiece(tmp->erow,tmp->ecol,-1,-1);
						return a;
					}
				}
			}
		}
		if(y==6){
			if((fy==(y-2)) && (fx==x)){
				if((board[fy][fx]==0)&&(board[y-1][x]==0)){
					a = 1;
				}
			}
			else if((fy==y-1)&&(fx==x)){
				if(board[fy][fx]==0){
					a = 1;
				}
			}
			else if((fy==y-1) && ((fx==x-1)||(fx==x+1))){
				if(board[fy][fx]>0){
					a = 1;
				}
			}
		}
		else if((fy==y-1)&&(fx==x)){
			if(board[fy][fx]==0){
				a = 1;
			}
		}
		else if((fy==y-1) && ((fx==x-1)||(fx==x+1))){
			if(board[fy][fx]>0){
				a = 1;
			}
		}
		else {
			a=0;
			wrefresh(chessboard);;
		}
		//if valid move i.e. a=1	
		if(a==1){
			if(fy==0){
			promotepawn(fy,fx);flipturn();movepiece(y,x,-1,-1);return 0;
			}
		}
	}
	return a;
}
//en-passant: when a pawn makes a double step from second row to the fourth and there is an enemy pawn on an adjacent square on the fourth row then this enemy pawn in the next move may move diagonally to the square that was passed over by the double stepping pawn. this must be done directly: if the player who could en passant does not do this in the first move after the double step, this pawn cannot be taken anymore by an en passant
// to promote a pawn
void promotepawn(int x,int y){
	int c;
	x:
	draw_borders(menuwin);
	mvwprintw(menuwin, 1,1 , "   WHAT WOULD YOU LIKE TO PROMOTE UR PAWN TO? press b for bishop press q for queen press r for rook press h for horse",A_STANDOUT);
	wrefresh(menuwin);
	c=wgetch(menuwin);
	wclear(menuwin);wrefresh(menuwin);wrefresh(chessboard);
	switch(c){
		case 'q':if(turn==0){
				board[x][y]=4;
				mvwprintw(chessboard,STRTBY+(3*x +2), STRTBX+(6*y +3),"Q2");
			}
				else{ 
			board[x][y]=-4;
			mvwprintw(chessboard,STRTBY+(3*x +2), STRTBX+(6*y +3),"Q1");
			}break;
		case 'r':
			if(turn==0){
				board[x][y]=1;
				mvwprintw(chessboard,STRTBY+(3*x +2), STRTBX+(6*y +3),"R2");
			}
			else{ 
			board[x][y]=-1;
			mvwprintw(chessboard,STRTBY+(3*x +2), STRTBX+(6*y +3),"R1");
			}break;
		case 'b':
			if(turn==0){
				board[x][y]=3;
				mvwprintw(chessboard,STRTBY+(3*x +2), STRTBX+(6*y +3),"B2");
			}
			else{ 
			board[x][y]=-3;
			mvwprintw(chessboard,STRTBY+(3*x +2), STRTBX+(6*y +3),"B1");
			}break;
		case 'h':
			if(turn==0){
				board[x][y]=2;
				mvwprintw(chessboard,STRTBY+(3*x +2), STRTBX+(6*y +3),"H2");
			}				
			else{
			board[x][y]=-2;
			mvwprintw(chessboard,STRTBY+(3*x +2), STRTBX+(6*y +3),"H1");
			}break;
		default: wclear(welcome);mvwprintw(welcome,10,10 , "enter correct option     ",A_STANDOUT);	
			board[x][y]=0;
		goto x;

	}
}
//horse
int horse(int x,int y,int x1,int y1){
	int a=0,m=0,killed=0;
	if((y1==y+2 && x1==x+1)||(y1==y+2 && x1==x-1)||(y1==y+1 && x1==x+2)||(y1==y+1 && x1==x-2)||(y1==y-1 && x1==x+2)||(y1==y-1 && x1==x-2)||(y1==y-2 && x1==x+1)||(y1==y-2 && x1==x-1)){
		a = 1;
	}
	return a;
}

//king
int king(int y,int x,int y1,int x1){
	int a = 0;	
		if((y1==y+1 && x1==x)||(y1==y-1 && x1==x)||(y1==y+1 && x1==x+1)||(y1==y+1 && x1==x-1)||(y1==y-1 && x1==x+1)||(y1==y-1 && x1==x-1)||(y1==y && x1==x+1)||(y1==y && x1==x-1)){
			a = 1;
		}
		//castling
		else if(board[y][x+1]==0&&board[y][x+2]==0&&(abs(board[y1][x1]) ==abs(board[y][x+3])==1||abs(board[y1][x1])== abs(board[y][x+3])==8)){
		return 1;
		} 
		if((turn==0)&&(board[y][x]<0)){
				a=0;
		}
		else if((turn!=0)&&(board[y][x]>0)){
				a=0;
		}
	return a; 
}
//bishop
int bishop(int y,int x,int y1,int x1) {
	int a=1,i;
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
		return a;
}
//queen
int queen(int x,int y,int x1,int y1){
	int a=0;
	if(x==x1||y==y1){
		//if queen moves in + direction
		a=1;
		return rook(x,y,x1,y1);
	}
	else if(abs(x1-x)==abs(y-y1)){
		//if queen moves in diagnol direction
		a=1;
		return bishop(x,y,x1,y1); 
	}
	if((turn==0)&&(board[x1][y1]<0)){
		a=0;
	}
	else if((turn!=0)&&(board[x1][y1]<0)){
		a=0;
	}
	return a;
} 
//rook
int rook(int x,int y,int x1,int y1){ 
	int i,a=1; 
	if(y1==y) {
		for(i=x+1;i<x1;i++) { 
			if(board[i][y1]!=0) { 
				a = 0;
				break;
			}
		}
		for(i=x-1;i>x1;i--) { 
			if(board[i][y1]!=0) {
				a = 0;
				break;
			}
		}
	}
	else if(x1==x) { 
		for(i=y+1;i<y1;i++) {
			if(board[x1][i]!=0) {
				a = 0;
				break;
			}
		}
		for(i=y-1;i>y1;i--) {
			if(board[x1][i]!=0) {
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

void movepiece(int y,int x, int fy,int fx) {
	int m,n,a;
	if((x==fx)&&(y==fy))
		return;
	if((x==-1)||(y==-1))
		goto x;
	if((x+y)%2==0){
		mvwaddch(chessboard,STRTBY+(3*y +2),STRTBX+(6*x +3), ACS_CKBOARD);
		mvwaddch(chessboard,STRTBY+(3*y +2),STRTBX+(6*x +4), ACS_CKBOARD);
	}
	else{
		mvwaddch(chessboard,STRTBY+(3*y +2),STRTBX+(6*x +3),' ');
		mvwaddch(chessboard,STRTBY+(3*y +2),STRTBX+(6*x +4),' ');
	}
	if(fx==-1||fy==-1){
		board[y][x]=0;
		return;}
			board[fy][fx]=board[y][x];
			board[y][x]=0;

	x:	switch(board[fy][fx]){
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
}
void printelement(){
	int i,i1=0,i2=0,xc=11,xc1=3;
	wclear(kwin);
	draw_borders(kwin);mvwprintw(kwin,1,1,"KILLED PIECES");wrefresh(kwin);
	for(i=0;i<count;i++){
		switch((karr[i])) {
			case 9:case 10:case 11:case 12:case 13:case 14:case 15:case 16:
				mvwprintw(kwin,3+i1,xc,"P2");wrefresh(kwin);i1++;
				break;
			case 2: case 7: 
				mvwprintw(kwin,3+i1,xc,"H2");wrefresh(kwin);i1++;
				break;
			case 4: 
				mvwprintw(kwin,3+i1,xc,"Q2");wrefresh(kwin);i1++;
				break;
			case 3:case 6:
				mvwprintw(kwin,3+i1,xc,"B2");wrefresh(kwin);i1++;
				break;
			case 1:case 8:
				mvwprintw(kwin,3+i1,xc,"R2");wrefresh(kwin);i1++;
				break;
			case 5: 
				mvwprintw(kwin,3+i1,xc,"K2");wrefresh(kwin);i1++;
				break;
			case -9:case -10:case -11:case -12:case -13:case -14:case -15:case -16:
				mvwprintw(kwin,3+i2,xc1,"P1");wrefresh(kwin);i2++;
				break;
			case -2: case -7:
				mvwprintw(kwin,3+i2,xc1,"H1");wrefresh(kwin);i2++;
				break;
			case -4: 
				mvwprintw(kwin,3+i2,xc1,"Q1");wrefresh(kwin);i2++;
				break;
			case -3:case -6:
				mvwprintw(kwin,3+i2,xc1,"B1");wrefresh(kwin);i2++;
				break;
			case -1:case -8:
				mvwprintw(kwin,3+i2,xc1,"R1");wrefresh(kwin);i2++;
				break;
			case -5: 
				mvwprintw(kwin,3+i2,xc1,"K1");wrefresh(kwin);i2++;
				break;	
		}
		if(i1==6){
			i1=0;
			xc+=2;
		}
		if(i2==6){
			i2=0;
			xc1+=2;
		}
	}
}

void flipturn(){
	turn=!(turn&&turn);
	if (turn==0){
		mvwprintw(chessboard,1,1,"Player 2 chance");
	}	
	else
		mvwprintw(chessboard,1,1,"Player 1 chance");
	wrefresh(chessboard);
}
void wrongstart(){
	mvwprintw(menuwin,10,3,"Wrong Input Press any key to continue...");
}
