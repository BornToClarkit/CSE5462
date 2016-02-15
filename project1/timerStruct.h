#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/time.h>
#include <string.h>

struct node{
  struct timeval delta_time;
  int port;
  int seq;
  struct node* next;
};
