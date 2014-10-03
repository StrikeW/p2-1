#ifndef _LIST_H_
#define _LIST_H_

#define INIT(HEAD) \
    (HEAD)->next = (HEAD); \
    (HEAD)->prev = (HEAD);

#define INSERT_REAR(HEAD, NODE) \
    ((HEAD)->prev)->next = (NODE); \
    (NODE)->prev = ((HEAD)->prev); \
    (NODE)->next = (HEAD);         \
    (HEAD)->prev = (NODE);

#define REMOVE_FRONT(HEAD) \
    (HEAD)->next = ((HEAD)->next)->next; \
    ((HEAD)->next)->prev = (HEAD);

#define REMOVE_NODE(NODE) \
    ((NODE)->next)->prev = (NODE)->prev; \
    ((NODE)->prev)->next = (NODE)->next;

#define FOREACH(HEAD, ITER) \
    for((ITER) = (HEAD)->next;  \
      (ITER) != (HEAD); (ITER) = (ITER)->next )

#define FRONT(HEAD)  ( (HEAD)->next )

#define EMPTY(HEAD)  ( (HEAD)->next == (HEAD) )

#define FOREACH(HEAD, ITER) \
    for((ITER) = (HEAD)->next;  \
      (ITER) != (HEAD); (ITER) = (ITER)->next )


typedef struct _list_head {
         struct _list_head *next;
         struct _list_head *prev;
}list_head;
 
 

int init(list_head *head);
void insert_rear(list_head *head, list_head *node);
int _empty(list_head *head);
list_head* delete_front(list_head *head);
int find(list_head *head, int tid);
list_head* remove_node(list_head *head, int tid);
int size(list_head *head);


#endif