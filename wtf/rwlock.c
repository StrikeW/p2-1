#include <syscall.h>
#include <thread.h>
#include <stdlib.h>
#include <assert.h>

#include "simics.h"
#include "cond_type.h"
#include "sem_type.h"
#include "mutex_type.h"
#include "rwlock_type.h"

int rwlock_init( rwlock_t *rwlock ){
	rwlock->read_cnt = 0;
	rwlock->write_cnt = 0;
	rwlock->status = RWLOCK_EXIST;
	if ((mutex_init(&rwlock->rd_cnt_mtx) < 0) || 
		(mutex_init(&rwlock->wr_cnt_mtx) < 0)){
		return RWLOCK_INIT_FAIL;
	}

	if ((cond_init(&rwlock->rd_cond) < 0 ) ||
		(cond_init(&rwlock->wr_cond) < 0 ) ||
		(cond_init(&rwlock->wait_rd_cond) < 0)){
		return RWLOCK_INIT_FAIL;
	}

	return RWLOCK_INIT_SUCC;
}

void rwlock_lock( rwlock_t *rwlock, int type ){
	switch(type){
		case RWLOCK_WRITE:

			mutex_lock(&rwlock->wr_cnt_mtx);
			while(rwlock->write_cnt > 0){
				cond_wait(&rwlock->wr_cond, &rwlock->wr_cnt_mtx);
			}

			/* Got the lock and set the count */
			rwlock->write_cnt = 1;
			mutex_unlock(&rwlock->wr_cnt_mtx);

			/* If there are some readers, wait again */
			mutex_lock(&rwlock->rd_cnt_mtx);
			while(rwlock->read_cnt > 0){
				cond_wait(&rwlock->wait_rd_cond, &rwlock->rd_cnt_mtx);
			}

			mutex_unlock(&rwlock->rd_cnt_mtx);

			break;

		case RWLOCK_READ:
			/* check if there is some writer and wait to finish */
			mutex_lock(&rwlock->wr_cnt_mtx);
			while(rwlock->write_cnt > 0){
				cond_wait(&rwlock->rd_cond, &rwlock->wr_cnt_mtx);
			}

			/* if no writers */
			mutex_lock(&rwlock->rd_cnt_mtx);
			rwlock->read_cnt++;
			mutex_unlock(&rwlock->rd_cnt_mtx);

			mutex_unlock(&rwlock->wr_cnt_mtx);

			break;
	}

	return;
}
void rwlock_unlock( rwlock_t *rwlock ){
	/* Unlock reader lock */
	if(rwlock->read_cnt > 0){
		mutex_lock(&rwlock->rd_cnt_mtx);

		rwlock->read_cnt--;

		/* if no reader anymore, signal to writer */
		if(rwlock->read_cnt == 0){
			cond_signal(&rwlock->wait_rd_cond);
		}

		mutex_unlock(&rwlock->rd_cnt_mtx);
	}/* Unlock a writer lock */
	else{
		mutex_lock(&rwlock->rd_cnt_mtx);
		mutex_lock(&rwlock->wr_cnt_mtx);

		rwlock->write_cnt = 0;
		
		/* Singal to the waiting writer */
		cond_signal(&rwlock->wr_cond);

		/* Broadcast to the waiting readers */
		cond_broadcast(&rwlock->rd_cond);

		mutex_unlock(&rwlock->wr_cnt_mtx);
		mutex_unlock(&rwlock->rd_cnt_mtx);
	}
	return;
}

void rwlock_destroy( rwlock_t *rwlock ){
	
	if(rwlock->read_cnt > 0|| rwlock->write_cnt > 0){
		lprintf("ERROR: Cannot destroy the rwlock!");
		return;
	}

	mutex_destroy(&rwlock->rd_cnt_mtx);
	mutex_destroy(&rwlock->wr_cnt_mtx);

	cond_destroy(&rwlock->rd_cond);
	cond_destroy(&rwlock->wr_cond);
	cond_destroy(&rwlock->wait_rd_cond);

	return;
}

void rwlock_downgrade( rwlock_t *rwlock ){
	
	mutex_lock(&rwlock->wr_cnt_mtx);
	mutex_lock(&rwlock->rd_cnt_mtx);

	/* Do downgrade */
	rwlock->read_cnt++;
	rwlock->write_cnt--;

	/* Singal the waiting writer */
	cond_signal(&rwlock->wr_cond);

	/* Broadcast to the waiting readers */
	cond_broadcast(&rwlock->rd_cond);

	mutex_unlock(&rwlock->rd_cnt_mtx);
	mutex_unlock(&rwlock->wr_cnt_mtx);
	
	return;
}


