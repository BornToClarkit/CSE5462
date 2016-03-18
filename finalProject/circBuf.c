#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "circBuf.h"

void initialize_circ_buf(circBuf *c, int size){
    c->data = malloc(sizeof(char)* size);
    c->size = size;
    c->dataHead = 0;
    c->dataTail = 0;
    c->full = 0;
    printf("size %i\n",c->size );
    fflush(stdout);
}

//tries to copy all bytes in msg, if the buffer would overflow then it copies as much as it can
//and returns total amount copied
int push_circ_buf(circBuf *c, char * msg, int msgLength){
    int copied = 0;
    printf("dataHead: %i\n", c->dataHead);
    printf("dataTail: %i\n", c->dataTail);
    if(c->dataHead > c->dataTail){
        if(msgLength <= c->size - c->dataHead){
            memcpy(&(c->data[c->dataHead]), msg, msgLength);
            c->dataHead += msgLength;
            copied = msgLength;
            return copied;
        }
        else{
            //copy to the end of buffer
            copied = c->size - c->dataHead;
            memcpy(&(c->data[c->dataHead]), msg, copied);
            c->dataHead = 0;
            int tmp;
            if(msgLength - copied < c->dataTail - c->dataHead){
                tmp = msgLength - copied;
            }else{
                tmp =  c->dataTail - c->dataHead;
                c->full = 1;
            }
            //copy around buffer either all of the data or to dataTail
            memcpy(&(c->data[c->dataHead]), msg + copied, tmp);
            copied +=tmp;
            return copied;
        }
    }
    else if(c->dataHead < c->dataTail){
        if(msgLength < c->dataTail - c->dataHead){
            memcpy(&(c->data[c->dataHead]), msg, msgLength);
            c->dataHead+= msgLength;
            copied = msgLength;
            return copied;
        }
        else{
            copied = c->dataTail - c->dataHead;
            memcpy(&(c->data[c->dataHead]), msg, copied);
            c->dataHead+=copied;
            c->full =1;
            return copied;
        }
    }
    else if(c->dataHead == c->dataTail){
        if(c->full == 1){
            printf("BUFFER IS FULL\n");
            return 0;
        }
        //initial case
        else {
            if(msgLength <= c->size - c->dataHead){
                memcpy(&(c->data[c->dataHead]), msg, msgLength);
                c->dataHead += msgLength;
                copied = msgLength;
                return copied;
            }
            else{
                //copy to the end of buffer
                copied = c->size - c->dataHead;
                memcpy(&(c->data[c->dataHead]), msg, copied);
                c->dataHead = 0;
                int tmp;
                if(msgLength - copied < c->dataTail - c->dataHead){
                    tmp = msgLength - copied;
                }else{
                    tmp =  c->dataTail - c->dataHead;
                }
                //copy around buffer
                memcpy(&(c->data[c->dataHead]), msg+ copied, tmp);
                copied +=tmp;
                return copied;
        }
    }
    }
}

//remove stuff from buffer
void remove_circ_buf(circBuf *c, int amount){
    c->dataTail = (c->dataTail + amount) % c->size;
    if(amount >0){
        c->full = 0;
    }
}

void print_buf(circBuf c){
    printf("Buffer: ");
    int i;
    for(i = 0; i < c.size; i++){
        printf("%c", c.data[i]);
    }
    printf("\n");
}
