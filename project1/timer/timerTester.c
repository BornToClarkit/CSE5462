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
#include "timerStruct.h"

#define LOCAL_PORT 6650
#define SEND_TO_PORT 7750

struct node* make_node(struct timeval delta_time, int port, int sequence, int flag);
void startTimer(double time, int sequence);
void cancelTimer(int sequence);

static int local_sock, send_to_sock; /* initial socket descriptor */
static struct sockaddr_in send_to_sin_addr;

int main(int argc, char *argv[]){
  struct sockaddr_in local_sin_addr;		/* structure for socket name setup */
  struct sockaddr_in src_addr;
  int src_addr_len;
  int addr_len;
  struct hostent *hp;
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
  if(getsockname(local_sock, (struct sockaddr *)&local_sin_addr, &src_addr_len) < 0){
    perror("getting sock name");
    exit(3);
  }
  printf("timerTester local port: %d\n", ntohs(local_sin_addr.sin_port));
  /////////////////////////////////////////////////////////////////////////

  if((send_to_sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
  {
    perror("error opening datagram socket");
    exit(1);
  }
  /* create name with parameters and bind name to socket */
  send_to_sin_addr.sin_family = AF_INET;
  send_to_sin_addr.sin_port = htons(SEND_TO_PORT);
  char beta[] = "COMPUTRON";
  hp = gethostbyname(beta);
  bcopy((void *)hp->h_addr, (void *)&send_to_sin_addr.sin_addr, hp->h_length);
  printf("timer test send_to_port : %d\n", ntohs(send_to_sin_addr.sin_port));

  startTimer(9, 1);
  sleep(1);
  startTimer(17,2);
  sleep(1);
  startTimer(1, 3);
  sleep(1);
  startTimer(3, 4);
  sleep(1);
  startTimer(4,5);
  sleep(1);
  cancelTimer(2);
  sleep(1);
  cancelTimer(4);
  cancelTimer(100);
}

//returns pointer to new node;
struct node* make_node(struct timeval delta_time, int port, int sequence, int flag){
  struct node* tmp;
  tmp = malloc(sizeof(struct node));
  tmp->delta_time = delta_time;
  tmp->port = port;
  tmp->info[0] = flag;
  memcpy(tmp->info + 1, &sequence, sizeof(int));
  tmp->next = NULL;
  return tmp;
}

void startTimer(double time, int sequence){
  char buf[sizeof(struct node)];		/* buffer for holding read data */
  int sec = (int)time;
  int usec = 1000000*(time - sec);
  struct timeval time1;
  time1.tv_sec = sec;
  time1.tv_usec = usec;
  struct node* timer = make_node(time1, LOCAL_PORT, sequence, 1);
  memcpy(buf, timer, sizeof(struct node));
  printf("Starting timer with time: %ld.%06ld, and seq_num: %i\n", timer->delta_time.tv_sec, timer->delta_time.tv_usec, sequence);
  sendto(local_sock, buf, sizeof(struct node) , 0, (struct sockaddr *)&send_to_sin_addr, sizeof(send_to_sin_addr));
}
//constructs a timer with 0.0 time and sends to timer-process
void cancelTimer(int sequence){
  char buf[sizeof(struct node)];		/* buffer for holding read data */
  struct timeval time1;
  time1.tv_sec = 0;
  time1.tv_usec = 0;
  struct node* timer = make_node(time1, LOCAL_PORT, sequence, 0);
  memcpy(buf, timer, sizeof(struct node));
  printf("Cancelling timer with seq_num: %i\n", sequence);
  sendto(local_sock, buf, sizeof(struct node) , 0, (struct sockaddr *)&send_to_sin_addr, sizeof(send_to_sin_addr));
}
