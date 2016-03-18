typedef struct circBuf{
    char* data; //buffer
    int size;   //size of buffer
    int dataHead;
    int dataTail;
    int full;
}circBuf;

void initialize_circ_buf(circBuf *c, int size);

int push_circ_buf(circBuf *c, char *msg, int msgLength);

void remove_circ_buf(circBuf *c, int amount);

void print_buf(circBuf c);
