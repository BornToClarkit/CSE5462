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
 	int sock;		/* initial socket descriptor */
	int msgsock;		/* accepted socket descriptor, each client connection has a unique socket descriptor*/
	struct sockaddr_in sin_addr;		/* structure for socket name setup */
	char buf[1024];		/* buffer for holding read data */
	int size_in_network[4];
	long long file_size;
	char name[20];
	char dest[27];
	int received = 0;
	int total_recv = 0;
	struct stat st;		/* used to determine if directory exists */			
	strcpy(dest,"sub/");
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
	if(recv(msgsock,size_in_network,4,MSG_WAITALL)<0)
	{
		perror("error reading on stream socket");
		exit(1);
	}
	file_size = ntohl(*size_in_network);
	printf("Server receives filesize: %d\n",file_size);
	if(recv(msgsock,name,20,MSG_WAITALL)<0)
	{
		perror("error reading on stream socket");
		exit(1);
	}
	printf("Server receives filename: %s\n",name);
	if(stat(dest, &st)!=0)
	{
		printf("Directory does not exist, making directory\n");
		if(mkdir(dest, 0777)!=0)
		{
			perror("ERROR: could not make sub directory");
			exit(1);
		}
	}
	char *location =strcat(dest,name);
	FILE * out = fopen(location,"wb");
	if(!out)
	{
		//printf("error opening the file to write output to... make sure there is a subdirectery called sub");
		//exit(1);
		
	}
	printf("File saved to this location: %s\n",location);
	/* put all zeros in buffer (clear) */
	bzero(buf,1024);
	/* read from msgsock and place in buf */
	while(total_recv < file_size)
	{
		received =recv(msgsock, buf, 1024,0);
		total_recv += received;
		if((fwrite(buf,1,received,out))!= received){
			perror("ERROR writing to file");
			exit(1);
		}
	}
	/* close all connections and remove socket file */
	close(msgsock);
	close(sock);
	fclose(out);
}
