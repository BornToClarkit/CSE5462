#include <sys/socket.h>
struct Packet{
	socklen_t address_len;
	struct sockaddr *address;
	int fileSize;
	char fileName[20];
	char message[1024];
};
ssize_t RECV(int socket, void *buffer, size_t length, int flags)
{
	//stuff
	recvfrom();
}

ssize_t SEND(int socket, const void*buffer, size_t length, int flags)
{
	//break up buffer struct that was sent through the buffer so that it fits into the 
	//	arguments of sendto()
	struct Packet *packet;
	socklen_t* address_length = NULL;
	struct sockaddr *address;
	bcopy(packet, buffer,length);

	
	sendto(socket,packet->message);
}

int CONNECT(void)
{

}

int ACCEPT(void)
{

}
