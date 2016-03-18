typedef struct{
    char* data; //buffer
    int size;   //size of buffer
    int dataHead;
}circBuf;

void initialize_circ_buf(circBuf *c, int size);

int push_circ_buf(circBuf *c, char *msg, int msgLength);
