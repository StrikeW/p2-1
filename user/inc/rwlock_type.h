/** @file rwlock_type.h
 *  @brief This file defines the type for reader/writer locks.
 */

#ifndef _RWLOCK_TYPE_H
#define _RWLOCK_TYPE_H

#include "mutex_type.h"
#include "cond_type.h"
#include "sem_type.h"

#define RWLOCK_INIT_FAIL -1
#define RWLOCK_INIT_SUCC 0

#define RWLOCK_EXIST 1
#define RWLOCK_DESTROY 0

typedef struct rwlock {
	int status;
	int read_cnt;
	int wirte_cnt;
	mutex_t rd_cnt_mtx;
	mutex_t wr_cnt_mtx;
	cond_t rd_cond;
	cond_t wr_cond;
} rwlock_t;


int rwlock_init( rwlock_t *rwlock );
// void rwlock_lock( rwlock_t *rwlock, int type );
// void rwlock_unlock( rwlock_t *rwlock );
// void rwlock_destroy( rwlock_t *rwlock );
// void rwlock_downgrade( rwlock_t *rwlock);



#endif /* _RWLOCK_TYPE_H */
