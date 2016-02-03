struct Packet{
	struct sockaddr *address;
	socklen_t address_len;
	int fileSize;
	char fileName[20];
	char message[1024];
};

ssize_t RECV(int socket, void *buffer, size_t length, int flags);

ssize_t SEND(int socket, const void*buffer, size_t length, int flags);

int CONNECT(void);

int Accept(void);


