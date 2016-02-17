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

#define LOCAL_PORT  7750

struct node* make_node(char * buf){
  struct node* tmp;
  tmp = malloc(sizeof(struct node));
  memcpy(tmp, buf, sizeof(struct node));
  return tmp;
}


//inserts node, may change the head node
void insert(struct node** insert_node, struct node** head){
  struct timeval insert_node_time = (**insert_node).delta_time;  //for easy access to time
  struct timeval head_time = (**head).delta_time;  //for easy access to time
  struct node* iterator_node = *head;
  int insert_seq_num;
  memcpy(&insert_seq_num, (**insert_node).info + 1, sizeof(int));
  struct timeval sum;
  sum.tv_sec = 0;
  sum.tv_usec = 0;
  struct timeval prev_sum;
  prev_sum.tv_sec = 0;
  prev_sum.tv_usec = 0;
  printf("inserting this node: ");
  printf("%ld.%06ld ", insert_node_time.tv_sec, insert_node_time.tv_usec);
  printf("seq_num: %i\n", insert_seq_num);
  //insert before head
  if(timercmp(&insert_node_time, &head_time, <)){
    (**insert_node).next = *head;
    timersub(&iterator_node->delta_time, &insert_node_time, &iterator_node->delta_time);
    timeradd(&sum, &iterator_node->delta_time, &sum);
    while(iterator_node != NULL){
      timersub(&sum,&prev_sum,&iterator_node->delta_time);
      iterator_node = iterator_node->next;
      if(iterator_node !=NULL){
        prev_sum = sum;
        timeradd(&sum, &iterator_node->delta_time, &sum);
      }
    }
    //update head node
    *head = *insert_node;
    //prevent double free
    *insert_node = NULL;
    return;
  }
  //determine where it goes in list and insert
  else{
    timeradd(&sum, &iterator_node->delta_time, &sum);
    prev_sum = sum;
    if(iterator_node->next != NULL){
      timeradd(&sum, &iterator_node->next->delta_time, &sum);
    }
    while(iterator_node->next != NULL){
      if(timercmp(&insert_node_time, &sum, <=)){
        (**insert_node).next = iterator_node->next;
        iterator_node->next = *insert_node;
        timersub(&(**insert_node).delta_time, &prev_sum, &(**insert_node).delta_time);
        timeradd(&prev_sum, &(**insert_node).delta_time, &prev_sum);
        iterator_node = iterator_node->next;
        iterator_node = iterator_node->next;
        //node has been inserted, now update nodes after insert node
        while(iterator_node!= NULL){
          timersub(&sum, &prev_sum, &iterator_node->delta_time);
          iterator_node = iterator_node->next;
          prev_sum = sum;
          if(iterator_node != NULL){
            timeradd(&sum, &iterator_node->delta_time, &sum);
          }
        }
        return;
      }
      iterator_node = iterator_node->next;
      prev_sum = sum;
      if(iterator_node->next != NULL){
        timeradd(&sum, &iterator_node->next->delta_time, &sum);
      }
    }
    //insert at end of list
    timersub(&(**insert_node).delta_time, &sum, &(**insert_node).delta_time);
    iterator_node->next = *insert_node;
    return;
  }
}

void delete(struct node** delete_node, struct node** head){
  int delete_seq_num;
  memcpy(&delete_seq_num, (**delete_node).info+1, sizeof(int));
  int iterator_seq_num;
  memcpy(&iterator_seq_num, (**head).info+1, sizeof(int));
  struct node* iterator_node = *head;
  printf("deleting the node with seq_num: %i\n", delete_seq_num);
  if(delete_seq_num == iterator_seq_num){
    *head = (**head).next;
    return;
  }
  while(iterator_node->next != NULL){
    memcpy(&iterator_seq_num, iterator_node->next->info+1, sizeof(int));
    if(iterator_seq_num == delete_seq_num){
      if(iterator_node->next->next != NULL){
        timeradd(&iterator_node->next->delta_time, &iterator_node->next->next->delta_time, &iterator_node->next->next->delta_time);
      }
      iterator_node->next= iterator_node->next->next;
      return;
    }
    iterator_node = iterator_node->next;
  }
}


void print_list(struct node* iterator_node){
  if(iterator_node == NULL){
    printf("List is empty\n");
    return;
  }
  while(iterator_node != NULL){
    int seq_num;
    memcpy(&seq_num, iterator_node->info+1, sizeof(int));
    printf("  %ld.%06ld, seq_num:%i\n", iterator_node->delta_time.tv_sec, iterator_node->delta_time.tv_usec, seq_num);
    iterator_node = iterator_node->next;
  }
}


int main(int argc, char *argv[]) {
  int local_sock;	/* initial socket descriptor */
  struct sockaddr_in local_sin_addr;		/* structure for socket name setup */
  struct sockaddr_in src_addr;
  int src_addr_len;
  int addr_len;
  char buf[sizeof(struct node)];		/* buffer for holding read data */
  struct timeval zero_time;
  zero_time.tv_sec = 0;
  zero_time.tv_usec = 0;
  if((local_sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
  {
    perror("error opening datagram socket");
    exit(1);
  }
  /* create name with parameters and bind name to socket */
  bzero(&local_sin_addr, sizeof(local_sin_addr));
  local_sin_addr.sin_family = AF_INET;
  local_sin_addr.sin_port = htons(LOCAL_PORT);
  local_sin_addr.sin_addr.s_addr = htonl(INADDR_ANY);

  if(bind(local_sock, (struct sockaddr *)&local_sin_addr, sizeof(local_sin_addr)) < 0) {
    perror("getting socket name");
    exit(2);
  }
  addr_len=sizeof(struct sockaddr_in);
  /* Find assigned port value and print it for client to use */
  if(getsockname(local_sock, (struct sockaddr *)&local_sin_addr, &src_addr_len) < 0){
    perror("getting sock name2");
    exit(3);
  }
  printf("timer-process local port: %d\n", ntohs(local_sin_addr.sin_port));
  /* put all zeros in buffer (clear) */
  bzero(buf,sizeof(struct node));
  struct node* head = NULL;
  struct node* node_recv = NULL;
  fd_set set;
  while(1){
    FD_ZERO(&set);
    FD_SET(local_sock, &set);
    if (select(local_sock + 1, &set, NULL, NULL, &head->delta_time) <0) {
      printf("\nSelect threw an exception\n");
      return 0;
    }
    if (FD_ISSET(local_sock, &set)){
      recvfrom(local_sock, buf, sizeof(struct node), 0, (struct sockaddr *)&src_addr , &src_addr_len);
      if(head == NULL){
        struct node* tmp = make_node(buf);
        if(tmp->info[0] == 1){
          printf("Before insert: \n");
          print_list(head);
          head = make_node(buf);
          printf("After insert\n");
        }
      }
      else{
        node_recv = make_node(buf);
        //insert
        if(node_recv->info[0]==1){
          printf("Before insert\n");
          print_list(head);
          insert(&node_recv, &head);
          printf("After insert\n");
        }//delete
        else if(node_recv->info[0]==0){
          printf("Before delete\n");
          print_list(head);
          delete(&node_recv, &head);
          printf("After delete\n");
        }
      }

    }
    else if (timercmp(&zero_time, &head->delta_time, ==)&& head!=NULL){
        int seq_num;
        memcpy(&seq_num, head->info+1, sizeof(int));
        printf("Timer with seq_num: %i has timed out. New list:\n", seq_num);
        if(head->next != NULL){
          head = head->next;
        }
        else{
          head = NULL;
        }
    }
    print_list(head);
  }
 
  return 0;
}
