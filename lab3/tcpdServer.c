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
#include "CapitalFunctions.h"

#define LOCAL_PORT 6666
#define REMOTE_PORT 1040
int main(int argc, char* argv[]){
/*initialize socket connection in unix domain*/
	int local_sock, remote_sock;		/* initial socket descriptor */
	struct sockaddr_in local_sin_addr;		/* structure for socket name setup */
	struct sockaddr_in remote_sin_addr;
	struct sockaddr_in src_addr;
	struct hostent *hp;
	char gamma[] = "gamma";
	hp = gethostbyname(gamma);
	int src_addr_len;
	int addr_len;
	char buf[1040];		/* buffer for holding read data */
	int *servermsg = NULL;
	servermsg = malloc(sizeof(int));
	if((local_sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
	{
		perror("error opening datagram socket");
		exit(1);
	}
	 /* create name with parameters and bind name to socket */
    local_sin_addr.sin_family = AF_INET;
    local_sin_addr.sin_port = htons(LOCAL_PORT);
	bcopy((void *)hp->h_addr, (void *)&local_sin_addr.sin_addr, hp->h_length);
    printf("tcpdServer local port: %d\n", ntohs(local_sin_addr.sin_port));

	/* put all zeros in buffer (clear) */
	bzero(buf,1040);
	/////////////////////////////////////////////////////////////////////////////////
	//remote port below here
	////////////////////////////////////////////////////////////////////////////////
    if((remote_sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
	{
		perror("error opening datagram socket");
		exit(1);
	}
	 /* create name with parameters and bind name to socket */
    remote_sin_addr.sin_family = AF_INET;
    remote_sin_addr.sin_port = htons(REMOTE_PORT);
    remote_sin_addr.sin_addr.s_addr = INADDR_ANY;

    if(bind(remote_sock, (struct sockaddr *)&remote_sin_addr, sizeof(remote_sin_addr)) < 0) {
			perror("getting socket name");
			exit(2);
    }
    addr_len=sizeof(struct sockaddr_in);
    /* Find assigned port value and print it for client to use */
    if(getsockname(remote_sock, (struct sockaddr *)&remote_sin_addr, &src_addr_len) < 0){
			perror("getting sock name2");
			exit(3);
    }
    printf("tcpdServer remote port: %d\n", ntohs(remote_sin_addr.sin_port));
	/* put all zeros in buffer (clear) */
	bzero(buf,1040);
    struct Packet packet;
	while(1){
		printf("received packet\n");
		ssize_t pie =recvfrom(remote_sock, buf, 1040, 0, (struct sockaddr *)&src_addr , &src_addr_len);
		printf("size: %d\n", pie);
		sendto(local_sock,buf,pie,0,(struct sockaddr *)&local_sin_addr,sizeof(local_sin_addr));
	}
	free(servermsg);
}
