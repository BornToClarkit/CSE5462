#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<errno.h>
#include<limits.h>
#include<sys/types.h>
#include<sys/socket.h>
#include <sys/stat.h>
#include<netinet/in.h>
#include<string.h>
#include "udp.h"

#define LOCAL_PORT 6666
#define REMOTE_PORT 9999
int main(int argc, char* argv[]){
/*initialize socket connection in unix domain*/
	int local_sock;		/* initial socket descriptor */
	struct sockaddr_in sin_addr;		/* structure for socket name setup */
	int addr_len;
	char buf[1000];		/* buffer for holding read data */
	if((local_sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
	{
		perror("error opening datagram socket");
		exit(1);
	}
	 /* create name with parameters and bind name to socket */
    sin_addr.sin_family = AF_INET;
    sin_addr.sin_port = LOCAL_PORT;
    sin_addr.sin_addr.s_addr = INADDR_ANY;
    if(bind(local_sock, (struct sockaddr *)&sin_addr, sizeof(sin_addr)) < 0) {
	perror("getting socket name");
	exit(2);
    }
    addr_len=sizeof(struct sockaddr_in);
    /* Find assigned port value and print it for client to use */
    if(getsockname(local_sock, (struct sockaddr *)&sin_addr, &addr_len) < 0){
	perror("getting sock name");
	exit(3);
    }
    printf("tcpdClient waiting on port # %d\n", sin_addr.sin_port);
	/* put all zeros in buffer (clear) */
	bzero(buf,1000);
	/* read from msgsock and place in buf */


}
