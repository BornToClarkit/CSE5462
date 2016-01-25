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
long get_port(int argc, char *argv[]){
	if(argc ==  2 && argv[1] != NULL){
		char * endptr;
		long val;
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

int main(int argc, char *argv[]){
	unsigned int server_s;	//server socked descriptor
	struct sockaddr_in server_addr;	// server internet address
	unsigned int connect_s;	// connection socket descriptor
	struct sockaddr_in client_addr;	// client internet address
	struct in_addr client_ip; // client ip address
	int addr_len;	// internet address length
	int port;	// local port-number
	if(port = get_port(argc, argv) < 0 ){
		exit(1);
	}
	server_s = socket(AF_INET, SOCK_STREAM, 0);
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(port);
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	if(bind(server_s, (struct sockaddr *)&server_addr, sizeof(server_addr)) <0){
		printf("Error could not bind socket\n");
		exit(1);
	}
	listen(server_s, 1);
	addr_len = sizeof(client_addr);
	//blocking accept connection call
	connect_s = accept(server_s, (struct sockaddr *) &client_addr, &addr_len);
	if(connect_s <0){
		printf("Error accepting connection\n");
		exit(1);
	}
	memcpy(&client_ip, &client_addr.sin_addr.s_addr, 4);
	printf("Accept completed\nIP of client: %s	port: %d \n", inet_ntoa(client_ip), ntohs(client_addr.sin_port));
}
