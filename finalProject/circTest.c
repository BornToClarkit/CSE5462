#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include<errno.h>
#include<limits.h>
#include <strings.h>
#include <string.h>
#include "circBuf.h"

int main(int argc, char *argv[]) {
    circBuf c;
    char buf[5]= "blah";
    char buf2[5]= "tree";
    initialize_circ_buf(&c, 6);
    printf("Amount of data put in buffer: %i\n", push_circ_buf(&c, buf, 5));
    printf("Amount of data put in buffer: %i\n", push_circ_buf(&c, buf2, 5));
    printf("done\n");

}
