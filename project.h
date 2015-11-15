#define STRTB 10
#define QUEEN 4
#define XMAX 48
#define YMAX 24
#define STRTBY 3
#define STRTBX 8
#define KING 5
int board[8][8];
typedef struct board_position{
	int row,col;
}bpos;
typedef struct windowsize {
	int length,breadth;
}winsize;
typedef struct lastmove{
	int sx,sy,ex,ey;
	int piece;
} lastmove;
typedef struct moves{
	int srow,scol,erow,ecol,prevval,newval;
	float c;
}moves;
typedef struct mstack{
	int top;
	moves arr[2];
}mstack;
typedef struct checkm{
	int x,y,piece;
}checkm;

lastmove umove; 
void quitgame();
void display_menu();
void printelement();
int board[8][8],count1,count2,killed;
bpos *position(int x,int y) ;
void undo();
void printboard(WINDOW *b);
void flipturn();
void game(int x, int y);
WINDOW *create_newwin(int height, int width, int starty, int startx);
void destroy_win(WINDOW *local_win);
void wrongstart();
void movepiece(int y,int x, int fy,int fx);
void initialize();
int gameover();
int pawn(int y,int x, int fy, int fx);
void promotepawn(int x,int y);
int horse(int x,int y,int x1,int y1);
int king(int x,int y,int x1,int y1);
 int bishop(int x,int y,int x1,int y1);
int queen(int x,int y,int x1,int y1);
int rook(int x,int y,int x1,int y1);void check();
