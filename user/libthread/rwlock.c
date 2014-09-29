#include <syscall.h>
#include <thread.h>
#include <stdlib.h>
#include <assert.h>

#include "simics.h"
#include "cond_type.h"
#include "sem_type.h"
#include "rwlock_type.h"


// int rwlock_init( rwlock_t *rwlock );
// void rwlock_lock( rwlock_t *rwlock, int type );
// void rwlock_unlock( rwlock_t *rwlock );
// void rwlock_destroy( rwlock_t *rwlock );
// void rwlock_downgrade( rwlock_t *rwlock);

// typedef struct rwlock {
// 	int status;
// 	int read_cnt;
// 	int wirte_cnt;
// 	mutex_t rd_cnt_mtx;
// 	mutex_t wr_cnt_mtx;
// 	cond_t rd_cond;
// 	cond_t wr_cond;
// } rwlock_t;

int rwlock_init( rwlock_t *rwlock ){
	rwlock->read_cnt = 0;
	rwlock->wirte_cnt = 0;
	rwlock->status = RWLOCK_EXIST;
	if(( mutex_init(&rwlock->rd_cnt_mtx) < 0) || 
		(mutex_init(&rwlock->wr_cnt_mtx) < 0)){
		return RWLOCK_INIT_FAIL;
	}

	if(( cond_init(&rwlock->rd_cond) < 0 ) ||
		(cond_init(&rwlock->wr_cond) < 0 )){
		return RWLOCK_INIT_FAIL;
	}

	return RWLOCK_INIT_SUCC;
}