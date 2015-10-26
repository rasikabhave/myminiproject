#include<stdio.h>
#include<stdlib.h>
#include<ncurses.h>
#include "project.h"
extern int turn;
extern WINDOW *debug,*chessboard;
//to check if the game is over
int gameover(){
	int i,j,a=0,b=0;
	for(i=0;i<8;i++)
		for(j=0;j<8;j++) {
			if(board[i][j]==5)
				a=1;
			if(board[i][j]==-5)
				b=1;
		}
	if(a==0){
		//player 1loses
		return 1;
	}
	if(b==0) {
		//player 2 loses
		return 1;
	}
	return 0;
}
//during the game
//when u get the coordinates of the click
void game(int x, int y){
	MEVENT ev;
	bpos *m;
	bool a;
	m=malloc(sizeof(bpos));//have to free this memory later
	int b,c,fx,fy;
	if(board[x][y]==0) {
		wprintw(debug,"1if1\t");wrefresh(debug);
		wrongstart();
		//goto x;
	}
	else {
		wprintw(debug,"2sac\t");wrefresh(debug);
		if(turn==0) {
			if(board[x][y]<0) {
				wrongstart();
			}
		}
		else if(turn!=0){
			if(board[x][y]>0){
				wrongstart();
			}
		}
	}wprintw(debug,"rasika");wrefresh(debug);
	//then the user should click in the final position and we need to find the corresponding box
c = wgetch(chessboard);
c = wgetch(chessboard);wprintw(debug,"rasia");wrefresh(debug);//c = wgetch(chessboard);
	switch(c)
	{
		case KEY_MOUSE:
			if(getmouse(&ev) == OK)
			{//wprintw(debug,"work!!!!!");wrefresh(debug);
				if(ev.bstate & BUTTON1_PRESSED)
				{wprintw(debug,"wtryneihdk");wrefresh(debug);
					fy=ev.y;fx=ev.x;
					//wmouse_trafo(chessboard,&fy,&fx,a);				
					m=position(fx,fy);
					fy=m->col;
					fx=m->row;mvwprintw(debug,2,2,"%d  %d",m->row,m->col);wrefresh(debug);
						if(!((m->row+1)*(m->col+1))){
							mvwprintw(debug,10,10,"not valid click again ");
							wrefresh(debug);return;
						}
						if((turn==0)&&(board[fx][fy]<0)){
							mvwprintw(debug,2,6,"choose num 2 ");
						}
						else if((turn!=0)&&(board[fx][fy]>0)){
							mvwprintw(debug,2,6,"choose num1");
						}
						wrefresh(debug);
						switch(abs(board[x][y])) {
							case 9:case 10:case 11:case 12:case 13:case 14:case 15:case 16:
									wprintw(debug,"entered pawn");wrefresh(debug);b=pawn(x,y,fx,fy);
									break;
							case 2: case 7: b=horse(x,y,fx,fy);
								break;
							case 4: b=queen(x,y,fx,fy);
								break;
							case 3:case 6:b=bishop(x,y,fx,fy);
								break;
							case 1:case 8: b=rook(x,y,fx,fy);
								break;
							case 5: b=king(x,y,fx,fy);
								break;
						}
						if(b==1){flipturn();
						mvwprintw(debug,15,5,"%d",turn);
						mvwprintw(debug,15,5,"%d",turn);
						wrefresh(debug);
						//movepiece(x,y,fx,fy);wrefresh(debug);
						umove.sx=x;umove.sy=y;umove.ex=fx;umove.ey=fy;umove.piece=board[fx][fy];
				}}
			}
	case 'u':undo();
	}
}
void flipturn()
{turn=!(turn&&turn);
}
void wrongstart(){
	wprintw(debug,"Wrong Input Press any key to continue...");
	//char ch=getch();	
//	if(ch==0){
//		getch();
//	}int i,j;
}


//check
void check(){
	int t=0,i,j,x1,y1;
	if(turn==0){
		for(i=0;i<=7;i++){
			for(j=0;j<=7;j++){
				if(board[i][j]==-5){
					y1=i;
					x1=j;
					break;
				}
			}
			for(j=0;j<=7;j++){
				for(i=0;i<=7;i++){
					if (t==1){
					//
						break;
					}
					if(board[j][i]>=9 && board[j][i]<=16)
						t = pawn(i,j,x1,y1);
					else if(board[j][i]==2 || board[j][i]==7)
						t = horse(i,j,x1,y1);
					else if(board[j][i]==4)
						t = queen(i,j,x1,y1);
					else if(board[j][i]==1 || board[j][i]==8)
						t = rook(i,j,x1,y1);
					else if(board[j][i]==3 || board[j][i]==6)
						t = bishop(i,j,x1,y1);
				 }
			}
	} }
	else{ 
		for(i=0;i<=7;i++){	
			for(j=0;j<=7;j++){
				if(board[i][j]==5){
					y1=i;
					x1=j;
					break;
				}
			}
		}	
		for(j=0;j<=7;j++){
			for(i=0;i<=7;i++){
				if (t==1){	
					//
					break;
				}
				if(board[j][i]>=(-9) && board[j][i]<=(-16))
					t = pawn(i,j,x1,y1); 
				else if(board[j][i]==(-2) || board[j][i]==(-7))
					t = horse(i,j,x1,y1);
				else if(board[j][i]==(-4))
					t = queen(i,j,x1,y1);
				else if(board[j][i]==(-1) || board[j][i]==(-8))
					t = rook(i,j,x1,y1);
				else if(board[j][i]==(-3) || board[j][i]==(-6))
					t = bishop(i,j,x1,y1); 
			}
		}
	}
}
void undo(){
movepiece(umove.sx,umove.sy,umove.ex,umove.ey);
}
