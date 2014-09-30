/** @file cond_type.h
 *  @brief This file defines the type for condition variables.
 */

#ifndef _COND_TYPE_H
#define _COND_TYPE_H

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "variable_queue.h"
#include "spin_lock.h"
#include "mutex_type.h"

#define COND_DESTORY 0
#define COND_EXIST 1

#define DEFAULT_TID -1

typedef struct cond {
	Q_NEW_LINK(cond) cond_link;
	spinlock_t spinlock;
	int tid;
	int status;
} cond_t;

/* Init the head of the waitting list */
Q_NEW_HEAD(waiting_list_t, cond);
/* The waitting list for threads */
waiting_list_t waiting_list;

int cond_init(cond_t *cond_var);
void cond_destroy(cond_t *cond_var);
void cond_wait(cond_t *cv, mutex_t *mp);
void cond_signal(cond_t *cv);
void cond_broadcast(cond_t *cv);

#endif /* _COND_TYPE_H */
