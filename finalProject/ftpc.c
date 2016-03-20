/* client.c using UDP */
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
#include <linux/tcp.h>
#include "CapitalFunctions.h"
//client

/*  Connor Clark
	Curtis Holton
*/


long check_args(int argc, char *argv[]){
	if(argc ==  4 && argv[1] != NULL && argv[2] != NULL && argv[3] != NULL)
	{
		if(strlen(argv[3]) >= 19)
		{
			printf("File name is too large\n");
			return -1;
		}
		return 0;
	}
	else
	{
		printf("Invalid number of arguments or an argument was null\ncommand should look like this:\n");
		printf("ftpc <remote-host> <port-number> <local-file-to-transfer> \n");
		return -1;
	}
}

//checks arg, returns port# if valid, if not returns -1;
int get_port(char *argv[]){
	if(argv[2] != NULL)
	{
		char * endptr;
		int val;
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

//used to find the size of the file
int get_file_size(FILE *fp){
	fseek(fp, 0, SEEK_END);
	int size = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	return size;
}

/* client program called with host name and port where server is run */
main(int argc, char *argv[])
{
	int sock;                     /* initial socket descriptor */
	int rval;                    /* returned value from a read */
	struct sockaddr_in sin_addr; /* structure for socket name
                                 * setup */
  struct sockaddr_in to_send;
  struct Packet packet;
	char buf[1060];
	struct hostent *hp;
	int pie;
	int *file_size = NULL;
	file_size = malloc(sizeof(int));
	char file_name[20];
	if(check_args(argc, argv)!=0)
	{
		exit(1);
	}
	strcpy(file_name, argv[3]);
	FILE *ifp = fopen(file_name, "rb");
	if(!ifp)
	{
		printf("test\n");
		printf("Could not open input file: \"%s\"\n", file_name);
		exit(1);
	}
	*file_size = get_file_size(ifp);
	/* initialize socket connection in unix domain */
	if((sock = SOCKET(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		perror("error opening socket");
		exit(1);
	}
	if(bind(sock, (struct sockaddr *)&sin_addr, sizeof(sin_addr)) < 0) {
		perror("ftpc: : getting socket name");
		exit(2);
    }
    printf("socket is bound\n");
	hp = gethostbyname(argv[1]);
	if(hp == 0)
	{
		fprintf(stderr, "%s: unknown host\n", argv[1]);
		exit(2);
	}
	/* construct name of socket to send to */
	bcopy((void *)hp->h_addr, (void *)&to_send.sin_addr, hp->h_length);
	to_send.sin_family = htons(AF_INET);
	if((to_send.sin_port = htons(get_port(argv))) < 0)
	{
		exit(1);
	}
	//
	/* establish connection with server */
	//copy file size into buffer
	*file_size = htonl(*file_size);
	memcpy(packet.buff, file_size,4);
	memcpy(file_size,packet.buff,4);

	//copy file name into buffer
	memcpy(packet.buff + sizeof(int), file_name ,20);
	memcpy(file_name,packet.buff+4,20);
	/* write buf to sock */
	packet.address = to_send;
	printf("port:%d\n",ntohs(packet.address.sin_port));
	if(SEND(sock,&packet,60,0) < 0)
	{
		perror("error writing on stream socket");
		exit(1);
	}
	printf("Size of tcp header: %d\n",sizeof(struct tcphdr));
	printf("Client sends file size: %i and filename: %s\n", ntohl(*file_size), file_name);
	int sent = 0;
	int total_sent = 0;
	while(total_sent < ntohl(*file_size)){
		sent = fread(packet.buff,1,1024,ifp);
		total_sent += sent;
		if(SEND(sock,&packet,sent+16+20,0)< 0)
		{
			perror("error writing on stream socket");
			exit(1);
		}
		sleep(1);
	}
	printf("total sent: %d\n",total_sent);
	printf("Client done sending file\n");
	free(file_size);
	fclose(ifp);
}
