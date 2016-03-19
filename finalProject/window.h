#include "circBuf.h"
typedef struct window{
    int * start;
    int * end;
    int seq_start; //sequence number of the first element
}window;

void initialize_window(window *w, circBuf *c);

void move_window(window *w, int amount);
