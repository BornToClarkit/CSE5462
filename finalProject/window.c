#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "window.h"
#include "circBuf.h"

void initialize_window(window *w, circBuf *c){
    w->start = c->data;
    w->end = w->start +20140;
}

void move_window(window *w, circBuf *c)
{
	if(w->start ==(&(c->data)+66780))
	{
		w->start=&(c->data);
	}
	else 
	{
		w->start+= 1060;
	}	
	
	if(w->end == (&(c->data)+66780))
	{
		w->end=&(c->data);
	}
	else
	{
		w->end+=1060;
	}
}
