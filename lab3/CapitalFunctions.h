struct Packet{
	struct sockaddr_in *address;
	socklen_t address_len;
	char buff[1024];
};

ssize_t RECV(int socket, void *buffer, size_t length, int flags);

ssize_t SEND(int socket, const void*buffer, size_t length, int flags);

int CONNECT(void);

int Accept(void);


