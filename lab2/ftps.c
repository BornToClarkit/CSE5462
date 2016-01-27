#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<errno.h>
#include<limits.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>
//server

//checks arg, returns port# if valid, if not returns -1;
int get_port(int argc, char *argv[]){
	if(argc ==  2 && argv[1] != NULL){
		char * endptr;
		int val;
		errno = 0;
		val = strtol(argv[1], &endptr, 10);
		if(errno !=0 && (val == LONG_MAX || val == LONG_MIN) || (errno !=0 && val==0) || endptr == argv[1] || val < 0)	{
			printf("Error converting port-number.\n");
			val = -1;
		}
		return val;
	}
	else {
		printf("Invalid number of arguments or an argument was null\ncommand should look like this:\n");
		printf("ftps <port-number>\n");
		return -1;
	}
}

/* server program called with no argument */
main(int argc, char* argv[])
{
 	int sock;                     /* initial socket descriptor */
	int msgsock;                  /* accepted socket descriptor,
                                 * each client connection has a
                                 * unique socket descriptor*/
	int rval=1;                   /* returned value from a read */  
	struct sockaddr_in sin_addr; /* structure for socket name setup */
	char buf[1024];               /* buffer for holding read data */
	char buf2[1024] = "Hello back in TCP from server"; 
	printf("TCP server waiting for remote connection from clients ...\n");
	/*initialize socket connection in unix domain*/
	if((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		perror("error openting datagram socket");
		exit(1);
	}
	/* construct name of socket to send to */
	sin_addr.sin_family = AF_INET;
	sin_addr.sin_addr.s_addr = INADDR_ANY;
	if((sin_addr.sin_port = htons(get_port(argc, argv))) < 0)
	{
		exit(1);
	}
	/* bind socket name to socket */
	if(bind(sock, (struct sockaddr *)&sin_addr, sizeof(struct sockaddr_in)) < 0) 
	{
		perror("error binding stream socket");
		exit(1);
	}
	/* listen for socket connection and set max opened socket connetions to 5 */
	listen(sock, 5);
	/* accept a (1) connection in socket msgsocket */ 
	if((msgsock = accept(sock, (struct sockaddr *)NULL, (int *)NULL)) == -1) 
	{ 
 	   perror("error connecting stream socket");
 	   exit(1);
	}
	/* put all zeros in buffer (clear) */
	bzero(buf,1024);
	/* read from msgsock and place in buf */
	if(read(msgsock, buf, 1024) < 0) 
	{
		perror("error reading on stream socket");
		exit(1);
	} 
	printf("Server receives: %s\n", buf);
	/* write message back to client */
	if(write(msgsock, buf2, 1024) < 0) 
	{
		perror("error writing on stream socket");
		exit(1);
	}
	printf("Server sends:    %s\n", buf2);
	/* close all connections and remove socket file */
	close(msgsock);
	close(sock);
}
