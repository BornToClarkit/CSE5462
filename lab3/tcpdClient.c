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
#define REMOTE_PORT 9980
int main(int argc, char* argv[]){
/*initialize socket connection in unix domain*/
	int local_sock, remote_sock;		/* initial socket descriptor */
	struct sockaddr_in local_sin_addr;		/* structure for socket name setup */
	struct sockaddr_in remote_sin_addr;
	struct sockaddr_in src_addr;
	int src_addr_len;
	int addr_len;
	struct hostent *hp;
	char buf[1000];		/* buffer for holding read data */
	if((local_sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
	{
		perror("error opening datagram socket");
		exit(1);
	}
	 /* create name with parameters and bind name to socket */
    local_sin_addr.sin_family = AF_INET;
    local_sin_addr.sin_port = htons(LOCAL_PORT);
    local_sin_addr.sin_addr.s_addr = INADDR_ANY;
    if(bind(local_sock, (struct sockaddr *)&local_sin_addr, sizeof(local_sin_addr)) < 0) {
	perror("getting socket name");
	exit(2);
    }
    addr_len=sizeof(struct sockaddr_in);
    /* Find assigned port value and print it for client to use */
    if(getsockname(local_sock, (struct sockaddr *)&local_sin_addr, &src_addr_len) < 0){
	perror("getting sock name");
	exit(3);
    }
    printf("tcpdClient local port: %d\n", ntohs(local_sin_addr.sin_port));
	/* put all zeros in buffer (clear) */
	bzero(buf,1000);
	/////////////////////////////////////////////////////////////////////////////////
	if((remote_sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
	{
		perror("error opening datagram socket");
		exit(1);
	}
	 /* create name with parameters and bind name to socket */
    remote_sin_addr.sin_family = AF_INET;
    remote_sin_addr.sin_port = htons(REMOTE_PORT);
    char beta[] = "beta";
		hp = gethostbyname(beta);
		bcopy((void *)hp->h_addr, (void *)&remote_sin_addr.sin_addr, hp->h_length);
    
    printf("tcpdClient remote port: %d\n", ntohs(remote_sin_addr.sin_port));
	/* put all zeros in buffer (clear) */
	bzero(buf,1000);
	
	//struct sockaddr_in tosend;
	//tosend.sin_family = AF_INET;
	//tosend.sin_port = 4444;
	//tosend.sin_addr.s_addr = INADDR_ANY;
	//struct Packet pac;
	//pac.address = local_sock;
	//char blah[] = "blah";
	//memcpy(pac.buff, blah, 5);
	
	
	
	while(1){
		printf("in loop\n");
		recvfrom(local_sock, buf, 1000, 0, (struct sockaddr *)&src_addr , &src_addr_len);
		//send to troll
		sendto(remote_sock, (char*)&buf, 1000, 0, (struct sockaddr *)&remote_sin_addr, sizeof(remote_sin_addr));
	}
}
