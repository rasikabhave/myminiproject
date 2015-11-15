/*	
********* Copyright (C) Rasika Bhave rasika.bhave5@gmail.com
 
	This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
    
    ****************************************** */
#include<stdio.h>
#include<stdlib.h>
#include<ncurses.h>
#include "project.h"
extern int turn;
int *karr;
int count=0;
extern int board[8][8];
extern WINDOW *menuwin,*chessboard;
moves move1;
extern mstack s;
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
		wclear(menuwin);
		mvwprintw(menuwin,5,10,"PLAYER 1 LOSES");//player 1loses
	}
	else if(b==0) {
		wclear(menuwin);
		mvwprintw(menuwin,5,10,"PLAYER 2 LOSES");//player 2 loses
	}
	wrefresh(menuwin);
	if(a==0||b==0)
		return 1;
	else
		return 0;
}
void undo(){
	movepiece(move1.erow,move1.ecol,move1.srow,move1.scol);
	board[move1.erow][move1.ecol]=move1.prevval;
	movepiece(-1,-1,move1.erow,move1.ecol);
	if((move1.prevval!=0)&&(move1.newval!=0))
		count--;
	printelement();
	check();
}
/*check where the king can move
if the king can move to a place will he be in check?
if yes continue to check other conditions 
else no checkmate*/
int checkmate(int row,int col,checkm arr[],int q)
{
	int ccount,valid,i,j,x,y,z,t=0,possible=0,n=0;
	y=row-1;
	x=col-1;
	while(x<0){
		x++;
	}	
	while(y<0)
		y++;
	if(turn!=0){
		while((y<=row+1)&&(x<=col+1)){
			valid=king(row,col,y,x);
			if((turn!=0)&&board[y][x]<0 || (x==8 || y==8)){
				valid=0;
				wprintw(menuwin,"   invalid turn 1");wrefresh(menuwin);
			}
			if(valid){	
				t=0;
				for(j=0;j<=7;j++){
					for(i=0;i<=7;i++){
							if(t==1){
								break;
							}
							if(board[j][i]>=9 && board[j][i]<=16)
								t = pawn(j,i,y,x);
							else if(board[j][i]==2 || board[j][i]==7)
								t = horse(j,i,y,x);
							else if(board[j][i]==4)
								t = queen(j,i,y,x);
							else if(board[j][i]==1 || board[j][i]==8)
								t = rook(j,i,y,x);
							else if(board[j][i]==3 || board[j][i]==6)
								t = bishop(j,i,y,x);
					}
				}
				if(t==1){
					ccount++;
				}
				else if(t==0){ 
					possible++;
				}
			}		
			else{
				ccount++;
			}
			if(y==row+1){
				x++;
			}
			if(y<row+1){
				y++;
			}
		}
		if(possible!=0){
			return;
		}
		for(z=0;z<q;z++)
		{	x=arr[z].x;y=arr[z].y;
			switch(abs(arr[z].piece))
			{	
				case 9:case 10:case 11:case 12:case 13:case 14:case 15:case 16:break;
				case 4:if(arr[z].y-row==arr[z].x-col)
						goto c3;
						//go to bishop case 
						if(arr[z].y==row||arr[z].x==col)
						//go to rook case
				case 1:case 8://rook
						if(row==arr[z].y) {
							for(i=arr[z].x+1;i<col;i++) { 
								if(board[row][i]==0) { 
									if(friendlymove(row,i)){
										possible++;goto end;}
								}
							}
							for(i=arr[z].x-1;i>col;i--) { 
								if(board[row][i]==0) {
									if(friendlymove(row,i)){
										possible++;goto end;}
								}
							}
						}
						else if(col==arr[x].x) { 
							for(i=arr[z].y+1;i<row;i++) {
								if(board[i][col]==0) {
									if(friendlymove(i,col)){
										possible++;goto end;}
								}
							}
							for(i=arr[z].y-1;i>col;i--) {
								if(board[i][col]==0) {
									if(friendlymove(i,col)){
										possible++;goto end;}
								}
							}
						}
						break;
						//generate mid way blocks and check if friendly piece can move in
				case 2:case 7://horse
				c3:case 3:case 6: //bishop
						if((x<col)&&(y<row)) { 
							for(i=1;(i+x)<col;i++) {
								if(board[y+i][x+i]==0) {
									if(friendlymove(y+i,x+i)){
										possible++;goto end;}
								}
							}
						}
						else if((x>col)&&(y>row)){ 
							for(i=1;(x-i)>col;i++){
								if(board[y-i][x-i]==0)
									if(friendlymove(y-i,x-i)){
										possible++;goto end;}
							}
						}
						else if((x>col)&&(y<row)){
							for(i=1;(x-i)>col;i++){ 
								if(board[y+i][x-i]==0) 
									if(friendlymove(y+i,x-i)){
										possible++;goto end;}
							} 
						}
						else if((x<col)&&(y>row)){
							for(i=1;(y-i)>row;i++){
								if(board[y-i][x+i]==0)
									if(friendlymove(y-i,x+i)){
										possible++;goto end;}
							}	
						}
						break;
			}
		}
	    	end:if(possible==0){
			mvwprintw(chessboard,2,30," player 1 CHECKMATE");wrefresh(chessboard);
		}
		else{	
			mvwprintw(chessboard,2,30,"                   ");wrefresh(chessboard);
		}
	}
	else if(turn==0){
		while((y<=row+1)&&(x<=col+1)){
			valid=king(row,col,y,x);
			if((turn==0)&&board[y][x]>0){
				valid=0;
			}
			if(valid){	
				t=0;
				for(j=0;j<=7;j++){
					for(i=0;i<=7;i++){
						if(t==1)
							break;
						if(board[j][i]>=-9 && board[j][i]<=-16)
							t = pawn(j,i,y,x);
						else if(board[j][i]==-2 || board[j][i]==-7)
							t = horse(j,i,y,x);
						else if(board[j][i]==-4)
							t = queen(j,i,y,x);
						else if(board[j][i]==-1 || board[j][i]==-8)
							t = rook(j,i,y,x);
						else if(board[j][i]==-3 || board[j][i]==-6)
							t = bishop(j,i,y,x);
					}
				}
				if(t==1){
					ccount++;
					}
				else if(t==0) {
					possible++;
				}
			}
			else {
				ccount++;
			}	
			if(y==row+1){
				x++;
			}
			if(y<row+1){
				y++;
			}
		}
		if(possible!=0)
			return;
		for(z=0;z<=q;z++)
		{	x=arr[z].x;y=arr[z].y;
			switch(arr[z].piece)
			{	
				case -9:case -10:case -11:case -12:case -13:case -14:case -15:case -16:break;
				case -4:if(arr[z].y-row==arr[z].x-col)
						goto c4;
						//go to bishop case 
					if(arr[z].y==row||arr[z].x==col)
					//go to rook case
				case -1:case -8://rook
					if(row==arr[z].y) {
							for(i=arr[z].x+1;i<col;i++){ 
								if(board[row][i]==0){ 
									if(friendlymove(row,i)){
										possible++;goto end;}
								}
							}
							for(i=arr[z].x-1;i>col;i--) { 
								if(board[row][i]==0) {
									if(friendlymove(row,i)){
										possible++;goto end;}
								}
							}
						}
						else if(col==arr[x].x) { 
							for(i=arr[z].y+1;i<row;i++) {
								if(board[i][col]==0) {
									if(friendlymove(i,col)){
										possible++;goto end;}
								}
							}
							for(i=arr[z].y-1;i>col;i--) {
								if(board[i][col]==0) {
									if(friendlymove(i,col)){
										possible++;goto end;}
								}
							}
						}
						break;
						//generate mid way blocks and check if friendly piece can move in
				case -2:case -7://horse
				c4:case -3:case -6: //bishop
						if((x<col)&&(y<row)) { 
								for(i=1;(i+x)<col;i++) {
									if(board[y+i][x+i]==0) {
										if(friendlymove(y+i,x+i)){
										possible++;goto end2;}
									}
								}
							}
							else if((x>col)&&(y>row)){ 
								for(i=1;(x-i)>col;i++){
									if(board[y-i][x-i]==0)
										if(friendlymove(y-i,x-i)){
										possible++;goto end2;}
								}
							}
							else if((x>col)&&(y<row)){
								for(i=1;(x-i)>col;i++){ 
									if(board[y+i][x-i]==0) 
										if(friendlymove(y+i,x-i)){
										possible++;goto end2;}
								} 
							}
							else if((x<col)&&(y>row)){
								for(i=1;(y-i)>row;i++){
									if(board[y-i][x+i]==0)
										if(friendlymove(y-i,x+i)){
										possible++;goto end2;}
								}	
							}
							break;
			}
		}
	    
		end2:if(possible==0){
			mvwprintw(chessboard,2,30," player 2 CHECKMATE");wrefresh(chessboard);
		}
		else{
				mvwprintw(chessboard,2,30,"                   ");wrefresh(chessboard);
		}
	}	
}
int friendlymove(int row,int col){
	int t2,t1,j,i;
	if(turn!=0){
		for(j=0;j<=7;j++){
			for(i=0;i<=7;i++){
				if (t2==1){	
					break;
				}
				if(board[j][i]>=(-9) && board[j][i]<=(-16)){
					flipturn();
					t2 = pawn(j,i,row,col);
					flipturn();
				} 
				else if(board[j][i]==(-2) || board[j][i]==(-7)){
					t2= horse(j,i,row,col);
				}
				else if(board[j][i]==(-4)){
					t2 = queen(j,i,row,col);
				}
				else if(board[j][i]==(-1) || board[j][i]==(-8))
					t2 = rook(j,i,row,col);
				else if(board[j][i]==(-3) || board[j][i]==(-6)){
						t2 = bishop(j,i,row,col);
				}
			}
		}
		if(t2==1)
			return 1;
		else 
			return 0;
	}
	if(turn==0){
		for(j=0;j<=7;j++){
			for(i=0;i<=7;i++){
				if (t2==1){	
						break;
				}
				if(board[j][i]>=(9) && board[j][i]<=(16)){
					flipturn();
					t2 = pawn(j,i,row,col);
					flipturn();
				} 
				else if(board[j][i]==(2) || board[j][i]==(7))
					t2= horse(j,i,row,col);
				else if(board[j][i]==(4)){
						t2 = queen(j,i,row,col);
				}
				else if(board[j][i]==(1) || board[j][i]==(8))
						t2 = rook(j,i,row,col);
				else if(board[j][i]==(3) || board[j][i]==(6)){
						t2 = bishop(j,i,row,col);
				}
			}
		}
		if(t2==1)
			return 1;
		else 
			return 0;
	}
}

//check
void check(){
	int t1=0,t2=0,i,j,x1,y1,z1=0,z2=0;
	checkm arr[30];
		for(i=0;i<=7;i++){
			for(j=0;j<=7;j++){
				if(board[i][j]==-5){
					y1=i;
					x1=j;
					break;
				}
			}
		}
		for(j=0;j<=7;j++){
			for(i=0;i<=7;i++){
				if (t1==1){
				}
				if(board[j][i]>=9 && board[j][i]<=16){
					flipturn();
					t1 = pawn(j,i,y1,x1);flipturn();
		//add position of pawn to a list
					if(t1==1){
						arr[z1].y=j;arr[z1].x=i;arr[z1].piece=board[j][i];z1++;
					}
				}
				else if(board[j][i]==2 || board[j][i]==7){
					t1 = horse(j,i,y1,x1);
					if(t1==1){
					arr[z1].y=j;arr[z1].x=i;arr[z1].piece=board[j][i];z1++;
					}
				}
				else if(board[j][i]==4){
					t1 = queen(j,i,y1,x1);
					if(t1==1){
					arr[z1].y=j;arr[z1].x=i;arr[z1].piece=board[j][i];z1++;
					}
				}
				else if(board[j][i]==1 || board[j][i]==8){
					t1 = rook(j,i,y1,x1);
					if(t1==1){
						arr[z1].y=j;arr[z1].x=i;arr[z1].piece=board[j][i];z1++;
					}
				}
				else if(board[j][i]==3 || board[j][i]==6){
					t1 = bishop(j,i,y1,x1);
					if(t1==1){
						arr[z1].y=j;arr[z1].x=i;arr[z1].piece=board[j][i];z1++;
					}
				}
			 }
		}
		if(z1!=0){
			mvwprintw(chessboard,1,25,"Player 1 in CHECK");wrefresh(chessboard);
			if(turn!=0)
				checkmate(y1,x1,&arr,z1);
			else{
				mvwprintw(chessboard,2,30,"                   ");wrefresh(chessboard);
			}
		}
		z2=0;
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
				if (t2==1){	
				}
				if(board[j][i]>=(-9) && board[j][i]<=(-16)){
					flipturn();
					t2 = pawn(j,i,y1,x1);flipturn();
					if(t2==1){
							arr[z2].y=j;arr[z2].x=i;arr[z2].piece=board[j][i];z2++;
					}
				}
				else if(board[j][i]==(-2) || board[j][i]==(-7)){
					t2= horse(j,i,y1,x1);
					if(t2==1){
						arr[z2].y=j;arr[z2].x=i;arr[z2].piece=board[j][i];z2++;
					}
				}
				else if(board[j][i]==(-4)){
					t2 = queen(j,i,y1,x1);
					if(t2==1){
						arr[z2].y=j;arr[z2].x=i;arr[z2].piece=board[j][i];z2++;
					}
				}
				else if(board[j][i]==(-1) || board[j][i]==(-8)){
					t2 = rook(j,i,y1,x1);
					if(t2==1){
						arr[z2].y=j;arr[z2].x=i;arr[z2].piece=board[j][i];z2++;
					}
				}
				else if(board[j][i]==(-3) || board[j][i]==(-6)){
					t2 = bishop(j,i,y1,x1);
					if(t2==1){
						arr[z2].y=j;arr[z2].x=i;arr[z2].piece=board[j][i];z2++;
					}
				} 
			}
		}
		wrefresh(chessboard);
		if(z2!=0){	
			mvwprintw(chessboard,1,25,"Player 2 in CHECK");wrefresh(chessboard);
			if(turn==0)
				checkmate(y1,x1,&arr,z2);
			else{
				mvwprintw(chessboard,2,30,"                   ");wrefresh(chessboard);
			}
		}
		if(z1==0&&z2==0) 
			mvwprintw(chessboard,2,25,"                 ");wrefresh(chessboard);
}

//during the game
//when u get the coordinates of the click
void game(int x, int y){
	MEVENT ev;
	bpos *m;
	bool a;
	m=malloc(sizeof(bpos));
	int b,c,fx,fy;
	if(board[x][y]==0) {
		wrongstart();
	}
	else {
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
	}
	//then the user should click in the final position and we need to find the corresponding box
	c = wgetch(chessboard);
	c = wgetch(chessboard);
	wrefresh(menuwin);
	switch(c)
	{
		case KEY_MOUSE:
			if(getmouse(&ev) == OK){
				if(ev.bstate & BUTTON1_PRESSED){
					fy=ev.y;fx=ev.x;
					m=position(fx,fy);
					fy=m->col;
					fx=m->row;
					if(!((m->row+1)*(m->col+1))){
						mvwprintw(menuwin,10,10,"not valid click again ");
						wrefresh(menuwin);return;
					}
					if((turn==0)&&(board[fx][fy]<0)){
						mvwprintw(menuwin,2,6,"choose num 2 ");
					}
					else if((turn!=0)&&(board[fx][fy]>0)){
						mvwprintw(menuwin,2,6,"choose num1");
					}
					wrefresh(menuwin);
					switch(abs(board[x][y])) {
						case 9:case 10:case 11:case 12:case 13:case 14:case 15:case 16:
									b=pawn(x,y,fx,fy);
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
						if(b==1){
							if((turn==0)&&board[x][y]<0)
								{return;}
							else if((turn!=0)&&board[x][y]>0)
								{return;}
							if((turn==0)&&board[fx][fy]>0)
								{return;}
							else if((turn!=0)&&board[fx][fy]<0)
								{return;}
							if((board[fx][fy]>0)&&(turn!=0)){
								karr[count]=board[fx][fy];count++;printelement();
							}
							else if((board[fx][fy]<0)&&(turn==0)){
								karr[count]=board[fx][fy];count++;printelement();
							}		
							wrefresh(menuwin);move1.prevval=board[fx][fy];
							move1.newval=board[x][y];
							movepiece(x,y,fx,fy);wrefresh(menuwin);
							move1.srow=x;move1.scol=y;move1.erow=fx;move1.ecol=fy;
							push(&s,move1);
							flipturn();check();
							display_menu();
						}
				}
			}
	}
}
