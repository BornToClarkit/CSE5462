typedef struct{
    int head;
    int tail;
    char data[64000];
}circBuf;

void initialize_circ_buf(circBuf *c, int size);

void push_circ_buf(circBuf *c, char *msg, int msgLength);
