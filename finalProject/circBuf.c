#include <stdio.h>
#include <stdlib.h>

typedef struct{
    char* data; //buffer
    int size;   //size of buffer
    int dataHead;
}circBuf;

void initialize_circ_buf(circBuf *c, int size){
    c->data = malloc(sizeof(char)* size);
    c->size = size;
    c->dataHead = 0;
    printf("size %i\n",c->size );
    fflush(stdout);
}

//copy as many bytes of the message that it can, the amount of bytes copied are
//returned
int push_circ_buf(circBuf *c, char * msg, int msgLength){
    int end = c->data + c->size;
    int copied = 0;
    // printf("start: %i,", c->data);
    // printf("end: %i\n", end);
    //too big to fit in array

    if(c->dataHead + msgLength > c->size){
        copied = c->size - c->dataHead;
        memcpy(c->data + c->dataHead, msg, copied);
        c->dataHead = c->dataHead + copied;
    }
    //fits in array
    else{
        memcpy(c->data + c->dataHead, msg, msgLength);
        c->dataHead = c->dataHead + msgLength;
        copied = msgLength;
    }
    print_buf(*c);
    return copied;
}


void print_buf(circBuf c){
    printf("Buffer: ");
    int i;
    for(i = 0; i < c.dataHead; i++){
        printf("%c", c.data[i]);
    }
    printf("\n");
}
