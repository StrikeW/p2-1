/** @file sem_type.h
 *  @brief This file defines the type for semaphores.
 */

#ifndef _SEM_TYPE_H
#define _SEM_TYPE_H

#include "mutex_type.h"
#include "cond_type.h"

#define SEM_DESTROY 0
#define SEM_EXIST 1

#define MUTEX_INIT_ERROR -1
#define CONDV_INIT_ERROT -1

#define SEM_INIT_SUCC 0 

typedef struct sem {
	int status;
	int count;
	mutex_t sem_mutex;
	cond_t sem_cond;
} sem_t;

int sem_init( sem_t *sem, int count );
void sem_wait( sem_t *sem );
void sem_signal( sem_t *sem );
void sem_destroy( sem_t *sem );


#endif /* _SEM_TYPE_H */
