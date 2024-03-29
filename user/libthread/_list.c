/** @file thread_queue.c
 *  @brief Internal thread queue library implementation.
 */

#include<stdio.h>
#include <_list.h>
#include<simics.h>

int
init(list_head *head)
{
    if(head == NULL)
        return -1;
    INIT(head);
    return 0;
}

list_head *
delete_front(list_head *head)
{
    list_head *front;

    if(EMPTY(head))
        return NULL;

    front = FRONT(head);
    REMOVE_FRONT(head);
    return front;
}

void
insert_rear(list_head *head, list_head *node)
{
    INSERT_REAR(head, node);
}

int
empty(list_head *head)
{
    return EMPTY(head);
}

list_head *
search(list_head *head, int tid)
{
    lprintf("Seach to be implemented");
    return NULL;
}

list_head *
remove_node(list_head *head, int tid)
{
    lprintf(" Remove node be implemented");
    return NULL;
}

int
size(list_head *head)
{
    int count = 0;
    list_head *iter;
    FOREACH(head, iter) {
        count ++;
    }
    return count;
}

