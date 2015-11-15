#include<stdio.h>
#include<stdlib.h>
#include<ncurses.h>
#include "project.h"
extern mstack s;
extern WINDOW *menuwin,*chessboard;
void init(mstack *s){
	s->top=-1;
	s->arr[0].newval=s->arr[0].prevval=s->arr[0].ecol=s->arr[0].srow=s->arr[0].scol=s->arr[0].erow=0;
	s->arr[1].newval=s->arr[1].prevval=s->arr[1].ecol=s->arr[1].srow=s->arr[1].scol=s->arr[1].erow=0;
}
void push(mstack *s,moves p){
	s->top=0;
		s->arr[0].newval=p.newval;
		s->arr[0].prevval=p.prevval;
		s->arr[0].srow=p.srow;
		s->arr[0].erow=p.erow;
		s->arr[0].scol=p.scol;
		s->arr[0].ecol=p.ecol;	
	if(s->top==0){
		s->arr[1].newval=p.newval;
		s->arr[1].prevval=p.prevval;
		s->arr[1].srow=p.srow;
		s->arr[1].erow=p.erow;
		s->arr[1].scol=p.scol;
		s->arr[1].ecol=p.ecol;		
		s->top=1;
	}
	
}
moves *mpop(mstack *s){
	moves *tmp=malloc(sizeof(moves));
	tmp->newval=s->arr[0].newval;
	tmp->prevval=s->arr[0].prevval;
	tmp->srow=s->arr[0].srow;
	tmp->erow=s->arr[0].erow;
	tmp->scol=s->arr[0].scol;
	tmp->ecol=s->arr[0].ecol;
	return tmp;
}
