#ifndef _LIST_H_
#define _LIST_H_

typedef struct _list_head {
        struct _list_head *next;
        struct _list_head *prev;
}list_head;


#define LIST_HEAD_INIT(name) { &(name), &(name) }
#define LIST_HEAD(name) \
        struct list_head name = LIST_HEAD_INIT(name)

#endif