#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/time.h>
#include <string.h>

#define INFO_SIZE 10
struct node{
  struct timeval delta_time;
  int port;
  char info[INFO_SIZE];
  struct node* next;
};
