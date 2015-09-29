#include<graphics.h>
#include<stdlib.h>
#include<stdio.h>
//#include<conio.h>
#include<ctype.h>

int board[8][8],turn=0,count1=0,count2=0;
int gameover();
char Filename[20];
void printBoard();
void wrongStart();
void initialize();
void cleartext();
void check();
int pawn(int x,int y,int x1,int y1);
int horse(int x, int y,int x1,int y1);
int rook(int x, int y,int x1,int y1);
int bishop(int x, int y,int x1,int y1);
int queen(int x, int y,int x1,int y1);
int king(int x, int y,int x1,int y1);

int main() {
	int gdriver=DETECT;
	int gmode;
	initgraph(&gdriver,&gmode,"C:\\TC\\BGI");
	int i;
	setbkcolor(BLACK);
	for(i=0;i<=6;i=i+1) {
		setcolor(RED);
		//settextstyle(11,0,i);
		outtextxy(200,200,"WELCOME TO CHESS");
		delay(500);
		cleardevice();
	}
	outtextxy(200,200,"WELCOME TO CHESS");
 circle(100,100,50);
//	settextstyle(1,0,3);
	getch();
	cleardevice();
}
void initialize(){
	  setcolor(WHITE);
	  rectangle(250,90,610,450);
		int i,j;
	  //Board Printing
	  for ( i=250;i<560;i+=90)
		for (j=90;j<450;j+=90)
		 {
		 bar(i,j,i+45,j+45);
		 bar(i+45,j+45,i+90,j+90);
		 }

/*	 Asigning the Values in the empty spaces*/
	   for(i=2;i<=5;i++)
		for( j=0;j<=7;j++)
		 board[i][j] = 0;

		for( j=0;j<=7;j++)
		{
		 board[0][j] = j+1;
		 board[1][j] = j+9;      //Pawns for player 1
		 board[7][j] = 0-(j+1);
		 board[6][j] = 0-(j+9);  //Pawns for player 2
		 }
	printboard();

	setcolor(YELLOW);

	  outtextxy(265,55,"A");
	  outtextxy(310,55,"B");
	  outtextxy(355,55,"C");
	  outtextxy(400,55,"D");
	  outtextxy(445,55,"E");
	  outtextxy(490,55,"F");
	  outtextxy(535,55,"G");
	  outtextxy(580,55,"H");
	  outtextxy(220,105,"1");
	  outtextxy(220,150,"2");
	  outtextxy(220,195,"3");
	  outtextxy(220,240,"4");
	  outtextxy(220,285,"5");
	  outtextxy(220,330,"6");
	  outtextxy(220,375,"7");
	  outtextxy(220,420,"8");
}
/*HORSE*/
int horse(int x,int y,int x1,int y1){
	int a=0;
	if((y1==y+2 && x1==x+1)||(y1==y+2 && x1==x-1)||(y1==y+1 &&
x1==x+2)||(y1==y+1 && x1==x-2)||(y1==y-1 && x1==x+2)||(y1==y-1 &&
x1==x-2)||(y1==y-2 && x1==x+1)||(y1==y-2 && x1==x-1)){
		a = 1;
	}
	return a;
}
/*KING*/

int king(int x,int y,int x1,int y1){
	int a = 0;
	if((y1==y+1 && x1==x)||(y1==y-1 && x1==x)||(y1==y+1 && x1==x+1)||(y1==y+1 && x1==x-1)||(y1==y-1 && x1==x+1)||(y1==y-1 &&x1==x-1)||(y1==y && x1==x+1)||(y1==y && x1==x-1)){
	a = 1;
	}
/*CASTLING*/
	else
	if(board[y][x+1]==0&&board[y][x+2]==0&&(abs(board[y1][x1])==abs(board[y][x+3])==1||abs(board[y1][x1])==abs(board[y][x+3])==8)){
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
/*QUEEN*/
int queen(int x,int y,int x1,int y1){
   if(x==x1||y==y1){               
  //if queen moves in + direction
	 return rook(x,y,x1,y1);/*rook function yet to be written*/
	}
   else if(abs(x1-x)==abs(y-y1)){ 
  //if queen moves in diagnol direction
	 return bishop(x,y,x1,y1);/*bishop function yet to be written*/
   }
   else
	 return 0;
}

/*PAWN*/
int pawn(int x,int y,int x1,int y1){
	int a=0;
	if(turn==0){
		if(y==1){
			if(y1==(y+2) && x1==x){
				if(board[y1][x1]==0&&board[y+1][x]==0){
						a = 1;
				}	
			}
		}
		if(y1==y+1 && x1==x){
			if(board[y1][x1]==0){
				a = 1;
			}
		}
		else if(y1==(y+1) && (x1==(x+1)||x1==(x-1))){
			if(board[y1][x1]<0){
				a = 1;
			}
		}
	}
	else if(turn==1){
		if(y==6){
			if( y1==(y-2) && x1==x){
				if(board[y1][x1]==0&&board[y-1][x]==0){
					a = 1;
			   	}
			}
		}
		if( y1==(y-1) && x1==x){
			if(board[y1][x1]==0){
				a = 1;
			}
		}
		else if(y1==(y-1) && (x1==(x-1)||x1==(x+1))){
			if(board[y1][x1]>0){
				a = 1;
			}
		}
	}
	if(a==1){
		if(turn==0){
			if(y1==7)
				return 2;
     		}
		else {
			if(y1==0)
				return 2;
		}
	}
	return a;
}

