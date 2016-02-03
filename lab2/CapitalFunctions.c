#include <sys/socket.h>

ssize_t RECV(int socket, void *buffer, size_t length, int flags)
{
	//stuff
	recvfrom();
}

ssize_t SEND(int socket, const void*buffer, size_t length, int flags)
{
	//break up buffer struct that was sent through the buffer so that it fits into the 
	//	arguments of sendto()
	sendto();
}

int CONNECT(void)
{

}

int ACCEPT(void)
{

}
