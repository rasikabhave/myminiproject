#include<stdio.h>
#include<stdlib.h>
#include<ncurses.h>
#define STRTB 10
#define QUEEN 4
#define XMAX 48
#define YMAX 24
#define KING 5
int board[8][8],turn=0,count1,count2,killed;
typedef struct board_position{
	int row,col;
}bpos;
typedef struct lastmove{
	int sx,sy,ex,ey;
	int piece;
} lastmove;
bpos *position(int x,int y) {
	bpos *b = malloc(sizeof(bpos));
	b->row=-1;
	b->col=-1;	
	if(y>=10){
		if((x>10)&&(x<16))
			b->col=0;
		if((x>16)&&(x<22))
			b->col=1;
		if((x>22)&&(x<28))
			b->col=2;
		if((x>28)&&(x<34))
			b->col=3;
		if((x>34)&&(x<40))
			b->col=4;
		if((x>40)&&(x<46))
			b->col=5;
		if((x>46)&&(x<52))
			b->col=6;
		if((x>52)&&(x<58))
			b->col=7;
	}
	if(x>=10){
		if((y>10)&&(y<13))
			b->row=0;
		if((y>13)&&(y<16))
			b->row=1;
		if((y>16)&&(y<19))
			b->row=2;
		if((y>19)&&(y<22))
			b->row=3;
		if((y>22)&&(y<25))
			b->row=4;
		if((y>25)&&(y<28))
			b->row=5;
		if((y>28)&&(y<31))
			b->row=6;
		if((y>31)&&(y<34))
			b->row=7;
	}
	return b;
	
}
void printboard();
WINDOW *create_newwin(int height, int width, int starty, int startx);
void destroy_win(WINDOW *local_win);
void wrongstart();
//to initialize the board
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
void abc(){int x,b,y,fx,fy;
	if(board[y][x]==0) {
		wrongstart();
	}
	else {
		if(turn==0) {
			if(board[y][x]<0) {
				wrongstart();
			}
		}
		else if(turn!=0){
			if(board[y][x]>0){
				wrongstart();
			}
		}
	}
	//then the user should click in the final position and we need to find the corresponding box
	if((turn==0)&&(board[fy][fx]>0)){
		//wrong move
	}
	else if((turn!=0)&&(board[fy][fx]<0)){
		//wrong move
	}
	switch(abs(board[y][x])) {
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
}
//pawn
int pawn(int x,int y, int fx, int fy) {
	int m,killed,a=0;
	mvprintw(10,10,"working");if(turn==0){
		if(y==1){
			if((fy==y+2)&&(fx==x)) {
				if(board[fy][fx]==0&&board[y+1][x]==0){
					a=1;
				}
			}
		}
		if((fy==y+1)&&(fx==x)) {
			if(board[fy][fx]==0){
				a=1;
			}
		}
		else if((fy==y+1)&&((fx==x+1)||(fx==x-1))){
			if(board[fy][fx]<0){
				a=1;
				killed=1;
			}
		}
//if valid move i.e. a=1	
		if(a==1){m=board[fy][fx];
			board[fy][fx]=board[y][x];
			board[y][x]=0;
			if(killed==1){
				//add m to a list of taken pieces
			}
		}
	//promote condition check where pawn is
		if(fy==7){
		//promotepawn(fx,fy)
		}
	}
	else if(turn!=0) {
		if(y==6){
			if((fy==(y-2)) && (fx==x)){
				if(board[fy][fx]==0&&board[y-1][x]==0){
					a = 1;
				}
			}
		}
		if((fy==y-1)&&(fx==x)){
			if(board[fy][fx]==0){
				a = 1;
			}
		}
		else if((fy==y-1) && ((fx==x-1)||(fx==x+1))){
			if(board[fy][fx]>0){
				a = 1;
				killed=1;
			}
		}
		//if valid move i.e. a=1	
		if(a==1){
			m=board[fy][fx];
			board[fy][fx]=board[y][x];
			board[y][x]=0;
			if(killed==1){
				//add m to a list of taken pieces
			}
		}
		if(fy==0){
			//promotepawn(fx,fy)
		}
	}
}//en-passant: when a pawn makes a double step from second row to the fourth and there is an enemy pawn on an adjacent square on the fourth row then this enemy pawn in the next move may move diagonally to the square that was passed over by the double stepping pawn. this must be done directly: if the player who could en passant does not do this in the first move after the double step, this pawn cannot be taken anymore by an en passant
// to promote a pawn
void promotepawn(int x,int y){
	//what would u like to promote it to and get the input
	board[y][x]=0;//whatever the user wants to promote it to
}
//horse
int horse(int x,int y,int x1,int y1){
	int a=0;
	if((y1==y+2 && x1==x+1)||(y1==y+2 && x1==x-1)||(y1==y+1 && x1==x+2)||(y1==y+1 && x1==x-2)||(y1==y-1 && x1==x+2)||(y1==y-1 && x1==x-2)||(y1==y-2 && x1==x+1)||(y1==y-2 && x1==x-1)){
		a = 1;
		if((board[y1][x1]<0)&&turn==0)
			killed=1;	
	}
	return a;
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
if(a==1)
 { if(board[y][x]==5) 
count1=1; 
if(board[y][x]==-5)
 count2=1;
 }
 return a; 
} //////BISHOP/////
 int bishop(int x,int y,int x1,int y1) {
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
	if(x==x1||y==y1){
		//if queen moves in + direction
		return rook(x,y,x1,y1);
	}
	else if(abs(x1-x)==abs(y-y1)){
		//if queen moves in diagnol direction
		return bishop(x,y,x1,y1); 
	}
	else return 0;
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
					//	setcolor(9);
					//	rectangle(20,335,100,365);
					//	outtextxy(30,340,"Check");
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
					//setcolor(GREEN);
					//rectangle(20,335,100,365);
					//outtextxy(30,340,"Check"); 
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

void wrongstart(){
	printw("\nWrong Input\nPress any key to continue...");
	char ch=getch();	
	if(ch==0){
		getch();
	}int i,j;
	for(i=1;i<=30;i++)
		for(j=1;j<=4;j++){
		//	gotoxy(i,j);
			//cout<<" ";
		}
		//gotoxy(1,1);
	//cout<<"enter initial position ¯ ";
}
int main(){
	WINDOW *my_win,*menu_win;
	int startx, starty, width, height;
	int ch,c,x,y,bx,by;
	initscr();	
	initialize();		/* Start curses mode 		*/
	mousemask(ALL_MOUSE_EVENTS, NULL);
	start_color();
	cbreak();
	noecho();
	getmaxyx(stdscr,y,x);			//Line buffering disabled, Pass on
	getbegyx(stdscr,by,bx);			 // everty thing to me 		*/
	keypad(stdscr, TRUE);		/* I need that nifty F1 	*/
	MEVENT event;
	bpos *m;
	//height = 3;
	//width = 10;
	my_win=create_newwin(64, 64, starty, startx);
	keypad(my_win,TRUE);
	starty = (LINES - height) /4 ;	/* Calculating for a center placement */
	startx = 27;	/* of the window		*/
	attron(A_BOLD);	
	mvwprintw(stdscr,starty,startx,"Welcome to Chess");
	attroff(A_BOLD);	
	//refresh();
	init_pair(1,COLOR_WHITE,COLOR_BLACK);
	wbkgd(stdscr,COLOR_PAIR(1));
	printboard();
	refresh();
	while(1){
		c = wgetch(stdscr);
		switch(c)
		{
		case KEY_MOUSE:
			if(getmouse(&event) == OK)
			{
			/* When the user clicks left mouse button */
				if(event.bstate & BUTTON1_PRESSED)
				{
				//	report_choice(event.x + 1, event.y + 1, &choice);
				//	if(choice == −1) //Exit chosen
				//		goto end;
					y=event.y;x=event.x;				
					//mvprintw(10,58,"Choice made is :%d   %d  String Chosen is ",y,x);
					m =position(x,y);
					if(!((m->row+1)*(m->col+1)))
						mvprintw(10,60,"not valid ");
					else
						mvprintw(10,60,"%d       %c",m->row+1,'A'+m->col);					
					refresh();
					
					pawn(x,y,x,y+1);
				}
			}break;
		case KEY_UP:wprintw(stdscr,"dsfdszx");
		break;	//print_menu(menu_win, choice);
			break;
		}
	}	endwin();
	
}
WINDOW *create_newwin(int height, int width, int starty, int startx)
{	WINDOW *local_win;

	local_win = newwin(height, width, starty*2, startx*2);
	box(local_win, 0 , 0);		/* 0, 0 gives default characters 
					 * for the vertical and horizontal
					 * lines			*/
	wrefresh(local_win);		/* Show that box 		*/

	return local_win;
}


//printboard
void printboard(){
	int i, j;
	for(i=0; i<=YMAX; i++)
		for(j=0; j<=XMAX; j++){
			if (i == 0)// First row of the gameboard.
			{	if (j == 0)// Upper-Left corner.
					mvaddch(STRTB+i, STRTB+j, ACS_ULCORNER);
				else if (j == XMAX)// Upper-Right corner.
					mvaddch(STRTB+i, STRTB+j, ACS_URCORNER);
				else if (j % (XMAX/8) == 0)   // Middle column.
					mvaddch(STRTB+i, STRTB+j, ACS_TTEE);
				else                    // Normal lines.
				mvaddch(STRTB+i, STRTB+j, ACS_HLINE);	
			}
			else if (i % (YMAX/8) == 0 && i != YMAX)   // Dividers.
			{
				if (j == 0)             // First column.
					mvaddch(STRTB+i, STRTB+j, ACS_LTEE);
				else if (j == XMAX)     // Last column.
					mvaddch(STRTB+i, STRTB+j, ACS_RTEE);
				else if (j % (XMAX/8) == 0)   // Middle column.
					mvaddch(STRTB+i, STRTB+j, ACS_PLUS);
				else                    // Normal lines.
					mvaddch(STRTB+i, STRTB+j, ACS_HLINE);
			}
			else if (i == YMAX)             // Last row
			{
				if (j == 0)              // Lower-Left corner.
					mvaddch(STRTB+i, STRTB+j, ACS_LLCORNER);
				else if (j == YMAX)     // Lower-Right corner.
					mvaddch(STRTB+i, STRTB+j, ACS_LRCORNER);
				else if (j % (XMAX/8)== 0)   // Middle column.
					mvaddch(STRTB+i, STRTB+j, ACS_BTEE);
				else                    // Normal lines.
					mvaddch(STRTB+i, STRTB+j, ACS_HLINE);
			}
			else if (j % (XMAX/8) == 0)   // Middle lines.
			{
				mvaddch(STRTB+i, STRTB+j, ACS_VLINE);
			}
			else {
				if(i%6<4)
					if(j%12<6)
						mvaddch(STRTB+i, STRTB+j, ACS_CKBOARD);
				if(i%6>3)
					if(j%12>6)
						mvaddch(STRTB+i, STRTB+j, ACS_CKBOARD);
			}
		}
		i=STRTB+1+24;int m=0;
		for(j=STRTB+3;j<STRTB+47;j+=6){
			mvaddch(i,j,'A'+m); 
			m++;       	
		}
		m=0;
		j=STRTB-1;		
		for(i=STRTB+1;i<STRTB+24;i+=3){
		mvaddch(i,j,'1'+m); 
		m++;	
		}
		init_pair(2,COLOR_RED,COLOR_BLACK);
		
		for(i=6;i<8;i++)
			for(j=0;j<8;j++){
				switch(abs(board[i][j])){
					case 9:case 10:case 11:case 12:case 13:case 14:case 15:case 16:
					mvprintw(10+(3*i +2),10+(6*j +3),"P1");
						break;
				case 2: case 7:	mvprintw(10+(3*i +2),10+(6*j +3),"H1");
						break;
				case 4:	mvprintw(10+(3*i +2),10+(6*j +3),"Q1");
					break;
				case 3:case 6:mvprintw(10+(3*i +2),10+(6*j +3),"B1");
					break;
				case 1:case 8: mvprintw(10+(3*i +2),10+(6*j +3),"R1");
					break;
				case 5: mvprintw(10+(3*i +2),10+(6*j +3),"K1");
					break;
				}
			}
		for(i=0;i<2;i++)
			for(j=0;j<8;j++){
				switch((board[i][j])){
					case 9:case 10:case 11:case 12:case 13:case 14:case 15:case 16:
					mvprintw(10+(3*i +2),10+(6*j +3),"P2");
						break;
				case 2: case 7:	mvprintw(10+(3*i +2),10+(6*j +3),"H2");
						break;
				case 4:	mvprintw(10+(3*i +2),10+(6*j +3),"Q2");
					break;
				case 3:case 6:mvprintw(10+(3*i +2),10+(6*j +3),"B2");
					break;
				case 1:case 8: mvprintw(10+(3*i +2),10+(6*j +3),"R2");
					break;
				case 5: mvprintw(10+(3*i +2),10+(6*j +3),"K2");
					break;
				}
			}
}
