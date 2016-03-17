#include <stdio.h>
#include <stdlib.h>

typedef struct{
    int head;
    int tail;
    char* data;
    char* data_end;
    int size;
}circBuf;

void initialize_circ_buf(circBuf *c, int size){
    c->head = 0;
    c->tail = 0;
    c->data = malloc(sizeof(char)* size);
    c->size = size;
    c->data_end = c->data;
}

void push_circ_buf(circBuf *c, char * msg){

}
