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
//returns pointer to new node;
struct node* make_node(struct timeval delta_time, int port, char* info){
  struct node* tmp;
  tmp = malloc(sizeof(struct node));
  tmp->delta_time = delta_time;
  tmp->port = port;
  memcpy(tmp->info, info, INFO_SIZE);
  tmp->next = NULL;
  return tmp;
}
//inserts node, may change the head node
void insert(struct node** insert_node, struct node** head){
  struct timeval insert_node_time = (**insert_node).delta_time;  //for easy access to time
  struct timeval head_time = (**head).delta_time;  //for easy access to time
  struct node* iterator_node = *head;
  struct timeval sum;
  sum.tv_sec = 0;
  sum.tv_usec = 0;
  struct timeval prev_sum;
  prev_sum.tv_sec = 0;
  prev_sum.tv_usec = 0;
  printf("inserting this node: ");
  printf("%ld.%06ld\n", insert_node_time.tv_sec, insert_node_time.tv_usec);
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
  struct node* iterator_node = *head;
  printf("deleting this node: ");
  printf("%ld.%06ld\n", (**delete_node).delta_time.tv_sec, (**delete_node).delta_time.tv_usec);
  //if delete node is head node
  if(*delete_node == *head){
    *head = (**head).next;
    return;
  }
  while(iterator_node->next != NULL){

  }

}

void print_list(struct node* iterator_node){
  while(iterator_node != NULL){
    printf("%ld.%06ld\n", iterator_node->delta_time.tv_sec, iterator_node->delta_time.tv_usec);
    iterator_node = iterator_node->next;
  }
}


int main(int argc, char const *argv[]) {
  struct node* head = NULL;
  struct node* blah = NULL;
  struct node* blah2 = NULL;
  struct node* blah3 = NULL;
  struct node* blah4 = NULL;
  struct node* blah5 = NULL;
  head = malloc(sizeof(struct node));
  blah = malloc(sizeof(struct node));
  blah2 = malloc(sizeof(struct node));
  blah3 = malloc(sizeof(struct node));
  blah4 = malloc(sizeof(struct node));
  blah5 = malloc(sizeof(struct node));
  struct timeval t1;
  t1.tv_sec = 4;
  t1.tv_usec = 0;
  struct timeval t2;
  t2.tv_sec = 9;
  t2.tv_usec = 0;
  head->delta_time = t1;
  blah->delta_time = t2;

  printf("Before insert\n");
  print_list(head);
  insert(&blah, &head);
  printf("After insert\n");
  print_list(head);

  struct timeval t3;
  t3.tv_sec = 17;
  t3.tv_usec=0;
  blah2->delta_time = t3;
  printf("Before insert\n");
  print_list(head);
  insert(&blah2, &head);
  printf("After insert\n");
  print_list(head);

  struct timeval t4;
  t4.tv_sec = 3;
  t4.tv_usec=0;
  blah3->delta_time = t4;
  printf("Before insert\n");
  print_list(head);
  insert(&blah3, &head);
  printf("After insert\n");
  print_list(head);

  struct timeval t5;
  t5.tv_sec = 9;
  t5.tv_usec=0;
  blah4->delta_time = t5;
  printf("Before insert\n");
  print_list(head);
  insert(&blah4, &head);
  printf("After insert\n");
  print_list(head);

  struct timeval t6;
  t6.tv_sec = 6;
  t6.tv_usec=500000;
  blah5->delta_time = t6;
  printf("Before insert\n");
  print_list(head);
  insert(&blah5, &head);
  printf("After insert\n");
  print_list(head);

  printf("Before delete\n");
  print_list(head);
  delete(&blah5, &head);
  printf("After delete\n");
  print_list(head);
  if(head!=NULL){
    free(head);
    head = NULL;
  }
  if(blah!=NULL){
    free(blah);
    blah = NULL;
  }
  if(blah2!=NULL){
    free(blah2);
    blah2 = NULL;
  }
  if(blah3!=NULL){
    free(blah3);
    blah3 = NULL;
  }
  if(blah4!=NULL){
    free(blah3);
    blah4 = NULL;
  }
  if(blah5!=NULL){
    free(blah3);
    blah4 = NULL;
  }
  return 0;
}
