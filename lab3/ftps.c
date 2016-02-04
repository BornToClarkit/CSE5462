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
#include "CapitalFunctions.h"
//server

/*  Connor Clark
	Curtis Holton
*/


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
 	int sock;		/* initial socket descriptor */
	struct sockaddr_in sin_addr;		/* structure for socket name setup */
	int addr_len;
	char buf[1040];		/* buffer for holding read data */
	int size_in_network[20];
	int file_size;
	char size2[4];
	char name[40];
	char dest[27];
	int size;
	int received = 0;
	int total_recv = 0;
	//printf("TCP server waiting for remote connection from clients ...\n");
	/*initialize socket connection in unix domain*/
	if((sock = SOCKET(AF_INET, SOCK_DGRAM, 0)) < 0)
	{
		perror("error opening datagram socket");
		exit(1);
	}
	
	
	 /* create name with parameters and bind name to socket */
    sin_addr.sin_family = AF_INET;
    sin_addr.sin_port = htons(get_port(argc, argv));
    sin_addr.sin_addr.s_addr = INADDR_ANY;
    if(bind(sock, (struct sockaddr *)&sin_addr, sizeof(sin_addr)) < 0) {
	perror("getting socket name");
	exit(2);
    }
    addr_len=sizeof(struct sockaddr_in);
    /* Find assigned port value and print it for client to use */
    if(getsockname(sock, (struct sockaddr *)&sin_addr, &addr_len) < 0){
	perror("getting sock name");
	exit(3);
    }
    printf("Server waiting on port # %d\n", ntohs(sin_addr.sin_port));
	/* put all zeros in buffer (clear) */
	bzero(buf,1000);
	/* read from msgsock and place in buf */
	if(RECV(sock,name,40,MSG_WAITALL)<0)
	{
		perror("error reading on datagram socket");
		exit(1);
	}

	struct Packet packet;
	memcpy(&packet,name,1040);
	memcpy(&size,packet.buff,4);
	size = ntohl(size);
	printf("Server receives filesize: %d\n",size);
	/*if(RECV(sock,name,20 + sizeof(struct sockaddr_in),MSG_WAITALL)<0)
	{
		perror("error reading on stream socket");
		exit(1);
	}
	*/
	printf("Server receives filename: %s\n",packet.buff+4);
	struct stat st;		/* used to determine if directory exists */			
	strcpy(dest,"sub/");
	if(stat(dest, &st)!=0)
	{
		printf("Directory does not exist, making directory\n");
		if(mkdir(dest, 0777)!=0)
		{
			perror("ERROR: could not make sub directory");
			exit(1);
		}
	}
	char *location =strcat(dest,packet.buff+4);
	FILE * out = fopen(location,"wb");
	if(!out)
	{
		printf("error opening the output file");
		exit(1);
		
	}
	printf("size of buffer: %d\n",sizeof(buf));
	printf("File saved to this location: %s\n",location);
	/* put all zeros in buffer (clear) */
	bzero(buf,1040);
	struct Packet pac;
	/* read from msgsock and place in buf */
	while(total_recv < size)
	{
		received =RECV(sock, buf, 1040,0);
		memcpy(&pac,buf,1040);
		total_recv += received;
		
		printf("stuff: %d\n",received);
		if((fwrite(pac.buff,1,received,out))!= received){
			perror("ERROR writing to file");
			exit(1);
		}
		printf("total received: %d\n",total_recv);
	}
	
	/* close all connections and remove socket file */
	close(sock);
	fclose(out);
}
