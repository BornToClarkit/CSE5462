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
#include "timerStruct.h"

#define LOCAL_PORT 6650
#define SEND_TO_PORT 7750
int main(int argc, char *argv[]){
  int local_sock, send_to_sock;		/* initial socket descriptor */
	struct sockaddr_in local_sin_addr;		/* structure for socket name setup */
  struct sockaddr_in send_to_sin_addr;
	struct sockaddr_in src_addr;
	int src_addr_len;
	int addr_len;
	struct hostent *hp;
	char buf[sizeof(struct node)];		/* buffer for holding read data */
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
    if(getsockname(local_sock, (struct sockaddr *)&local_sin_addr, &src_addr_len) < 0){
	perror("getting sock name");
	exit(3);
    }
    printf("timerTester local port: %d\n", ntohs(local_sin_addr.sin_port));
	/* put all zeros in buffer (clear) */
	bzero(buf,sizeof(struct node));
/////////////////////////////////////////////////////////////////////////

  if((send_to_sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
  {
    perror("error opening datagram socket");
    exit(1);
  }
   /* create name with parameters and bind name to socket */
    send_to_sin_addr.sin_family = AF_INET;
    send_to_sin_addr.sin_port = htons(SEND_TO_PORT);
    char beta[] = "COMPUTRON";
  hp = gethostbyname(beta);
  bcopy((void *)hp->h_addr, (void *)&send_to_sin_addr.sin_addr, hp->h_length);
    printf("timer test send_to_port : %d\n", ntohs(send_to_sin_addr.sin_port));
  /* put all zeros in buffer (clear) */
  bzero(buf,sizeof(struct node));


  char info[] = "blah";
  struct node* time1 = NULL;
  time1 = malloc(sizeof(struct node));
  time1->delta_time.tv_sec = 10;
  time1->delta_time.tv_usec = 0;
  time1->port = 6650;
  memcpy(time1->info, info, 4);
  memcpy(buf, time1, sizeof(struct node));
  sendto(local_sock, buf, sizeof(struct node) , 0, (struct sockaddr *)&send_to_sin_addr, sizeof(send_to_sin_addr));
  free(time1);
}
