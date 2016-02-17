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
struct Packet{
	struct sockaddr_in address;
	char buff[1024];
};
ssize_t RECV(int socket, void *buffer, size_t length, int flags)
{
	struct sockaddr_in src_addr;
	int src_addr_len;
	struct Packet packet;
	ssize_t pie =recvfrom(socket, buffer, length, flags, (struct sockaddr *)&src_addr , &src_addr_len);
	pie = pie - 16;
	return pie;
}

ssize_t SEND(int socket, const void*buffer, size_t length, int flags)
{
	//break up buffer struct that was sent through the buffer so that it fits into the 
	//	arguments of sendto()
	struct sockaddr_in daemon;
	struct Packet packet;
	struct hostent *h2;
	h2 = gethostbyname("beta");
	bcopy((void *)h2->h_addr, (void *)&daemon.sin_addr, h2->h_length);
	daemon.sin_family = AF_INET;
	if((daemon.sin_port = htons(6650)) < 0)
	{
		exit(1);
	}
	ssize_t sent =sendto(socket,buffer,length,0,(struct sockaddr*)&daemon,sizeof(daemon));
	return sent;
}

int CONNECT(void)
{

}

int ACCEPT(void)
{

}
int SOCKET(int domain, int type, int protocol){
	return socket(AF_INET, SOCK_DGRAM, 0);
}