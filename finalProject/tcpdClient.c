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

#define LOCAL_PORT 6650
#define REMOTE_PORT 9980
#define CRC16 0x8005
// source:http://stackoverflow.com/questions/10564491/function-to-calculate-a-crc16-checksum
uint16_t gen_crc16(const uint8_t *data, uint16_t size)
{
    uint16_t out = 0;
    int bits_read = 0, bit_flag;

    /* Sanity check: */
    if(data == NULL)
        return 0;

    while(size > 0)
    {
        bit_flag = out >> 15;

        /* Get next bit: */
        out <<= 1;
        out |= (*data >> bits_read) & 1; // item a) work from the least significant bits

        /* Increment bit counter: */
        bits_read++;
        if(bits_read > 7)
        {
            bits_read = 0;
            data++;
            size--;
        }

        /* Cycle check: */
        if(bit_flag)
            out ^= CRC16;

    }

    // item b) "push out" the last 16 bits
    int i;
    for (i = 0; i < 16; ++i) {
        bit_flag = out >> 15;
        out <<= 1;
        if(bit_flag)
            out ^= CRC16;
    }

    // item c) reverse the bits
    uint16_t crc = 0;
    i = 0x8000;
    int j = 0x0001;
    for (; i != 0; i >>=1, j <<= 1) {
        if (i & out) crc |= j;
    }

    return crc;
}
int main(int argc, char* argv[]){
/*initialize socket connection in unix domain*/
	int local_sock, remote_sock;		/* initial socket descriptor */
	struct sockaddr_in local_sin_addr;		/* structure for socket name setup */
	struct sockaddr_in remote_sin_addr;
	struct sockaddr_in src_addr;
	struct sockaddr_in troll;
	struct sockaddr_in empty;
	int src_addr_len;
	int addr_len;
	struct hostent *hp;
	char buf[1060];		/* buffer for holding read data */
	if((local_sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
	{
		perror("error opening datagram socket");
		exit(1);
	}
	 /* create name with parameters and bind name to socket */
    local_sin_addr.sin_family = AF_INET;
    local_sin_addr.sin_port = htons(LOCAL_PORT);
    local_sin_addr.sin_addr.s_addr = INADDR_ANY;
    if(bind(local_sock, (struct sockaddr *)&local_sin_addr, sizeof(local_sin_addr)) < 0) {
	perror("getting socket name");
	exit(2);
    }
    addr_len=sizeof(struct sockaddr_in);
    /* Find assigned port value and print it for client to use */
    if(getsockname(local_sock, (struct sockaddr *)&local_sin_addr, &src_addr_len) < 0){
	perror("getting sock name");
	exit(3);
    }
    printf("tcpdClient local port: %d\n", ntohs(local_sin_addr.sin_port));
	/* put all zeros in buffer (clear) */
	bzero(buf,1060);
	/////////////////////////////////////////////////////////////////////////////////
	//remote port below here
	////////////////////////////////////////////////////////////////////////////////
	if((remote_sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
	{
		perror("error opening datagram socket");
		exit(1);
	}
	 /* create name with parameters and bind name to socket */
    remote_sin_addr.sin_family = AF_INET;
    remote_sin_addr.sin_port = htons(REMOTE_PORT);
    char beta[] = "beta";
	hp = gethostbyname(beta);
	bcopy((void *)hp->h_addr, (void *)&remote_sin_addr.sin_addr, hp->h_length);
    printf("tcpdClient remote port: %d\n", ntohs(remote_sin_addr.sin_port));
    struct Packet crc;
    
	/* put all zeros in buffer (clear) */
	bzero(buf,1060);
	int i = 0;
	while(1){
		ssize_t pie = recvfrom(local_sock, buf, 1060, 0, (struct sockaddr *)&src_addr , &src_addr_len);
		//send to troll
		
		memcpy(&crc,buf,pie);
		crc.TCPHeader.check = 0;
		memcpy(buf,&crc,pie);
		crc.TCPHeader.check= gen_crc16(buf+16,pie-16);
		memcpy(buf,&crc,pie);
		i++;
		printf("Packet:     size: %d    CRC:   %d\n",i, pie,crc.TCPHeader.check);
		printf("\n");
		
		sendto(remote_sock, buf, pie, 0, (struct sockaddr *)&remote_sin_addr, sizeof(remote_sin_addr));
		printf("sent packet\n");
	}
}
