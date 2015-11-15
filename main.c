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

#include <ncurses.h>
#include <stdio.h>
#include<stdlib.h>
#include "project.h"
int turn=0;
mstack s;
extern int *karr;
extern int count;
WINDOW *welcome,*chessboard,*menuwin,*kwin;
	bpos *b;
int parent_x, parent_y;
winsize wel,cb,kp;
void initialize(){
	int i,j;
	for(i=2;i<6;i++)
		for(j=0;j<8;j++)
			board[i][j]=0;
	for(j=0;j<8;j++){
		board[0][j]=j+1;
		board[1][j]=j+9;
		board[7][j]=0-(j+1);
		board[6][j]=0-(j+9);
	}
}
void printboard( WINDOW *b);
void draw_borders(WINDOW *screen);
int main(int argc, char *argv[]) {
	int c,x,y;
	bpos *m;
	init(&s);
	new:	initscr();//initialize the screen
	initialize();//initialize the board
	noecho();
	start_color();
	init_pair(1,COLOR_WHITE,COLOR_BLACK);
	init_pair(2,COLOR_RED,COLOR_WHITE);	
	cbreak();
	mousemask(ALL_MOUSE_EVENTS, NULL);
	bool a;
	keypad(stdscr, TRUE);		
	MEVENT event;	
	getmaxyx(stdscr, parent_y, parent_x);
	wel.breadth=parent_x/2;
	wel.length=2*parent_y/5;
	cb.breadth=10*parent_x/16-10;
	cb.length=parent_y-10;
	welcome = newwin(wel.length,wel.breadth, 3*parent_y/10,4*parent_x/16 ); //initilaize welcome window
	chessboard = newwin(cb.length,cb.breadth, 3, 5);//initialixe board window
	menuwin=newwin(20,30,3,cb.breadth+6);//init menuwin window	
	keypad(chessboard, TRUE);
	keypad(welcome, TRUE);
	wbkgd(welcome,COLOR_PAIR(2));		
	draw_borders(welcome);
	mvwprintw(welcome, wel.length/2-1,wel.breadth/4 , "WELCOME TO THE GAME OF CHESS        ",A_STANDOUT);
	wrefresh(welcome);
	sleep(5);
	wclear(welcome);
	wrefresh(welcome);
	draw_borders(welcome);
	mvwprintw(welcome, wel.length/2-1,wel.breadth/3 , "   1.START A NEW GAME\n                           2.EXIT     ",A_STANDOUT);
	wrefresh(welcome);c=wgetch(welcome);
	wclear(welcome);wrefresh(welcome);delwin(welcome);
	kwin=newwin(11,30,23,cb.breadth+6);
	draw_borders(menuwin);
	wrefresh(menuwin);
	if(c=='1'){
	draw_borders(chessboard);	
	printboard(chessboard);
	wrefresh(chessboard);
	}
	if(c=='2'){wclear(menuwin);wrefresh(menuwin);
		welcome = newwin(wel.length,wel.breadth, 3*parent_y/10,4*parent_x/16 );draw_borders(welcome);
		mvwprintw(welcome, wel.length/2-1,wel.breadth/4 , "Thank you for trying our game",A_STANDOUT);
		wrefresh(welcome);wgetch(welcome);
		goto end;
	
	}
	
	display_menu();
	wrefresh(menuwin);
	wbkgd(chessboard,COLOR_PAIR(1));
	b= malloc(sizeof(bpos));
	karr=(int *)malloc(sizeof(int)*30);
	flipturn();
	while(!gameover()){
			c = wgetch(chessboard);
			switch(c)
			{
				case KEY_MOUSE:
					if(getmouse(&event) == OK)
					{
					/* When the user clicks left mouse button */
						if(event.bstate & BUTTON1_PRESSED){
							y=event.y;x=event.x;	
							m =position(x,y);
							if(!((m->row+1)*(m->col+1))){
									mvwprintw(chessboard,1,45," not valid ");
								wrefresh(chessboard);
							}
							else{
								mvwprintw(chessboard,1,45,"chosen :%d %c",m->row+1,'A'+m->col);
								wrefresh(chessboard);
								game(m->row,m->col);						
							}
						}
					}
					break;
				case 'u':case 'U':undo();
						flipturn();	
						wrefresh(chessboard);wrefresh(menuwin);
						break;	
				case 'N':case 'n':wclear(menuwin);
					if(turn==0)
					mvwprintw(menuwin,1,3,"Player 2:Are you sure you want to start a new game? :(");
					else
					mvwprintw(menuwin,1,3,"Player 1:Are you sure you want to start a new game? :(");
					mvwprintw(menuwin,4,3,"Y/N");wrefresh(menuwin);
					c=wgetch(menuwin);
					switch(c){
						case 'y':case 'Y':
							wclear(menuwin);wclear(kwin);turn=0;wrefresh(menuwin);wrefresh(kwin);
							wclear(chessboard);wrefresh(chessboard);
							goto new;
							break;
						case 'n':case 'N':display_menu();
							break;
					}break;
				case 'q':case 'Q':quitgame();goto end;break;
				}
			
	}
	end:wgetch(chessboard);wclear(welcome);wrefresh(welcome);wclear(menuwin);wrefresh(menuwin);
	delwin(chessboard);
	endwin();
	return 0;
}
void quitgame(){
	int x=3,y=2;
	char c;wclear(menuwin);draw_borders(menuwin);
	if(turn==0){
	mvwprintw(menuwin,y++,x,"Player 2: Are you sure you want to quit the game? :(");
	}
	else
	mvwprintw(menuwin,y++,x,"Player 1: Are you sure you want to quit the game? :(");
	mvwprintw(menuwin,++y,x,"Y/N");
	wrefresh(menuwin);
	c=wgetch(menuwin);
	switch(c){
		case 'y':case 'Y':
			mvwprintw(menuwin,y++,x,"Do you wish to start a new game or exit?");
			mvwprintw(menuwin,++y,x,"Press N for new game / E for exit");
			c=wgetch(menuwin);
			switch(c){
				case 'n':case 'N':// goto new;return;
							break;
				case 'e':case 'E':wclear(chessboard);wclear(kwin);wclear(menuwin);
						wrefresh(chessboard);wrefresh(kwin);wrefresh(menuwin);
						welcome = newwin(wel.length,wel.breadth, 3*parent_y/10,4*parent_x/16 );draw_borders(welcome);
						mvwprintw(welcome, wel.length/2-1,wel.breadth/4 , "Thank you for trying our game",A_STANDOUT);
						wrefresh(welcome);break;
			}
			break;
		case 'n':case 'N':display_menu();
				return;
	}
	return;
}	
void display_menu(){
	wclear(menuwin);
	draw_borders(menuwin);
	int x=3,y=2;
	mvwprintw(menuwin,y++,x,"MENU");
	mvwprintw(menuwin,y++,x,"1.New Game");
	mvwprintw(menuwin,y++,x,"Press N");
	mvwprintw(menuwin,y++,x,"2.Undo move");
	mvwprintw(menuwin,y++,x,"Press U");
	mvwprintw(menuwin,y++,x,"3.Quit game");
	mvwprintw(menuwin,y++,x,"Press Q");
	wrefresh(menuwin);
}
void draw_borders(WINDOW *screen) {
	int x, y, i;
	getmaxyx(screen, y, x);
	mvwprintw(screen, 0, 0, "+");
	mvwprintw(screen, y - 1, 0, "+");
	mvwprintw(screen, 0, x - 1, "+");
	mvwprintw(screen, y - 1, x - 1, "+");
	    // sides
	for (i = 1; i < (y - 1); i++) {
		mvwprintw(screen, i, 0, "|");
		mvwprintw(screen, i, x - 1, "|");
	}
	// top and bottom
	for (i = 1; i < (x - 1); i++) {
		mvwprintw(screen, 0, i, "-");
		mvwprintw(screen, y - 1, i, "-");
	}
}
void printboard(WINDOW *b){
	int i, j;
	for(i=0; i<=YMAX; i++)
		for(j=0; j<=XMAX; j++){
			if (i == 0)// First row of the gameboard.
			{	if (j == 0)// Upper-Left corner.
					mvwaddch(b,STRTBY+i, STRTBX+j, ACS_ULCORNER);
				else if (j == XMAX)// Upper-Right corner.
					mvwaddch(b,STRTBY+i, STRTBX+j, ACS_URCORNER);
				else if (j % (XMAX/8) == 0)   // Middle columns.
					mvwaddch(b,STRTBY+i, STRTBX+j, ACS_TTEE);
				else  // Normal lines.
				mvwaddch(b,STRTBY+i, STRTBX+j, ACS_HLINE);	
			}
			else if (i % (YMAX/8) == 0 && i != YMAX)   // Dividers.
			{
				if (j == 0) // First column.
					mvwaddch(b,STRTBY+i, STRTBX+j, ACS_LTEE);
				else if (j == XMAX)     // Last column.
					mvwaddch(b,STRTBY+i, STRTBX+j, ACS_RTEE);
				else if (j % (XMAX/8) == 0)   // Middle columns.
					mvwaddch(b,STRTBY+i, STRTBX+j, ACS_PLUS);
				else  // Normal lines.
					mvwaddch(b,STRTBY+i, STRTBX+j, ACS_HLINE);
			}
			else if (i == YMAX) // Last row
			{
				if (j == 0)  // Lower-Left corner.
					mvwaddch(b,STRTBY+i, STRTBX+j, ACS_LLCORNER);
				else if (j == YMAX)     // Lower-Right corner.
					mvwaddch(b,STRTBY+i, STRTBX+j, ACS_LRCORNER);
				else if (j % (XMAX/8)== 0)   // Middle columns.
					mvwaddch(b,STRTBY+i, STRTBX+j, ACS_BTEE);
				else // Normal lines.
					mvwaddch(b,STRTBY+i, STRTBX+j, ACS_HLINE);
			}
			else if (j % (XMAX/8) == 0)   // Middle lines.
			{
				mvwaddch(b,STRTBY+i, STRTBX+j, ACS_VLINE);
			}
			else {
				if(i%6<4)
					if(j%12<6)
						mvwaddch(b,STRTBY+i, STRTBX+j, ACS_CKBOARD);
				if(i%6>3)
					if(j%12>6)
						mvwaddch(b,STRTBY+i, STRTBX+j, ACS_CKBOARD);
			}
		}
		i=STRTBY+1+24;int m=0;
		for(j=STRTBX+3;j<STRTBX+47;j+=6){
			mvwaddch(b,i,j,'A'+m); 
			m++; 	
		}
		m=0;
		j=STRTBX-1;		
		for(i=STRTBY+1;i<STRTBY+24;i+=3){
			mvwaddch(b,i,j,'1'+m); 
			m++;	
		}
		init_pair(2,COLOR_RED,COLOR_BLACK);
		for(i=6;i<8;i++)
			for(j=0;j<8;j++){
				switch(abs(board[i][j])){
					case 9:case 10:case 11:case 12:case 13:case 14:case 15:case 16:
					mvwprintw(b,STRTBY+(3*i +2),STRTBX+(6*j +3),"P1");
						break;
				case 2: case 7:	mvwprintw(b,STRTBY+(3*i +2),STRTBX+(6*j +3),"H1");
						break;
				case 4:	mvwprintw(b,STRTBY+(3*i +2),STRTBX+(6*j +3),"Q1");
					break;
				case 3:case 6:mvwprintw(b,STRTBY+(3*i +2),STRTBX+(6*j +3),"B1");
					break;
				case 1:case 8: mvwprintw(b,STRTBY+(3*i +2),STRTBX+(6*j +3),"R1");
					break;
				case 5: mvwprintw(b,STRTBY+(3*i +2),STRTBX+(6*j +3),"K1");
					break;
				}
			}
		for(i=0;i<2;i++)
			for(j=0;j<8;j++){
				switch((board[i][j])){
					case 9:case 10:case 11:case 12:case 13:case 14:case 15:case 16:
					mvwprintw(b,STRTBY+(3*i +2),STRTBX+(6*j +3),"P2");
						break;
				case 2: case 7:	mvwprintw(b,STRTBY+(3*i +2),STRTBX+(6*j +3),"H2");
						break;
				case 4:	mvwprintw(b,STRTBY+(3*i +2),STRTBX+(6*j +3),"Q2");
					break;
				case 3:case 6:mvwprintw(b,STRTBY+(3*i +2),STRTBX+(6*j +3),"B2");
					break;
				case 1:case 8: mvwprintw(b,STRTBY+(3*i +2),STRTBX+(6*j +3),"R2");
					break;
				case 5: mvwprintw(b,STRTBY+(3*i +2),STRTBX+(6*j +3),"K2");
					break;
				}
			}
}

bpos *position(int x,int y) {
	b->row=-1;
	b->col=-1;	
	if(y>=STRTBY+3){
		if((x>STRTBX)&&(x<STRTBX+5+6))
			b->col=0;		
		if((x>STRTBX+5+6)&&(x<STRTBX+5+12))
			b->col=1;
		if((x>STRTBX+5+12)&&(x<STRTBX+5+18))
			b->col=2;
		if((x>STRTBX+5+18)&&(x<STRTBX+5+24))
			b->col=3;
		if((x>STRTBX+5+24)&&(x<STRTBX+5+30))
			b->col=4;
		if((x>STRTBX+5+30)&&(x<STRTBX+5+36))
			b->col=5;
		if((x>STRTBX+5+36)&&(x<STRTBX+5+42))
			b->col=6;
		if((x>STRTBX+5+42)&&(x<STRTBX+5+48))
			b->col=7;
			}
	if(x>=STRTBX+5){
		if((y>STRTBY+3)&&(y<STRTBY+3+3))
			b->row=0;
		if((y>STRTBY+3+3)&&(y<STRTBY+3+6))
			b->row=1;
		if((y>STRTBY+3+6)&&(y<STRTBY+3+9))
			b->row=2;
		if((y>STRTBY+3+9)&&(y<STRTBY+3+12))
			b->row=3;
		if((y>STRTBY+3+12)&&(y<STRTBY+3+15))
			b->row=4;
		if((y>STRTBY+3+15)&&(y<STRTBY+3+18))
			b->row=5;
		if((y>STRTBY+3+18)&&(y<STRTBY+3+21))
			b->row=6;
		if((y>STRTBY+3+21)&&(y<STRTBY+3+24))
			b->row=7;
	}
	return b;
}
