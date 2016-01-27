/* client.c using TCP */
/* Client for connecting to Internet stream server waiting on port 1040 */
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
#define port "1040"   /* socket file name */

long check_args(int argc, char *argv[]){
	if(argc ==  4 && argv[1] != NULL && argv[2] != NULL && argv[3] != NULL)
	{
		return 0;
	}
	else 
	{
		printf("Invalid number of arguments or an argument was null\ncommand should look like this:\n");
		printf("ftpc <remote-IP> <port-number> <local-file-to-transfer> \n");
		return -1;
	}
}

//checks arg, returns port# if valid, if not returns -1;
long get_port(char *argv[]){
	if(argv[2] != NULL)
	{
		char * endptr;
		long val;
		errno = 0;
		val = strtol(argv[2], &endptr, 10);
		if(errno !=0 && (val == LONG_MAX || val == LONG_MIN) || (errno !=0 && val==0) || endptr == argv[1] || val < 0)	
		{
			printf("Error converting port-number.\n");
			val = -1;
		}
		return val;
	}
	else 
	{
		return -1;
	}
}

/* client program called with host name where server is run */
main(int argc, char *argv[])
{
	if(check_args(argc, argv)!=0)
	{
		exit(1);
	}
	int sock;                     /* initial socket descriptor */
	int  rval;                    /* returned value from a read */
	struct sockaddr_in sin_addr; /* structure for socket name 
                                 * setup */
	char buf[1024] = "Hello in TCP from client";     /* message to set to server */
	struct hostent *hp;
	/* initialize socket connection in unix domain */
	if((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		perror("error opening socket");
		exit(1);
	}
	hp = gethostbyname(argv[2]);
	if(hp == 0) 
	{
		fprintf(stderr, "%s: unknown host\n", argv[1]);
		exit(2);
	}
	/* construct name of socket to send to */
	bcopy((void *)hp->h_addr, (void *)&sin_addr.sin_addr, hp->h_length);
	sin_addr.sin_family = AF_INET;
	if(sin_addr.sin_port = get_port(argv) < 0)
	{
		exit(1);
	}
	printf("%u\n", sin_addr.sin_port);
	/* establish connection with server */
	if(connect(sock, (struct sockaddr *)&sin_addr, sizeof(struct sockaddr_in)) < 0) 
	{
		close(sock);
		perror("error connecting stream socket");
		exit(1);
	}
  
	/* write buf to sock */
	if(write(sock, buf, 1024) < 0) 
	{
		perror("error writing on stream socket");
		exit(1);
	}
	printf("Client sends:    %s\n", buf);
	if(read(sock, buf, 1024) < 0) 
	{
		perror("error reading on stream socket");
		exit(1);
	}
	printf("Client receives: %s\n", buf);
}
