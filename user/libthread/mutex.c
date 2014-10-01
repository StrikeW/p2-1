/** @file mutex.c
 *
 *  @brief Implementation of mutex
 *  
 *  @bug No known bug
 */
#include <syscall.h>
#include <assert.h>
#include <stdio.h>

#include "simics.h"
#include "mutex_type.h"

extern int atom_xchg(int*, int);
/** @brief Mutex initialization
 *  
 *  @param  mp: mutex
 *  @return 0: success
 */
int mutex_init(mutex_t *mp){
	if( mp == NULL){
		lprintf("ERROR: Illegal mutex to init!");
		return MUTEXT_INIT_FAIL;
	}
	mp->lock = UNLOCK;
	mp->status = MUTEX_EXIST;
	return MUTEXT_INIT_SUCC;
}

/** @brief Lock a mutex
 *  
 *  @param  mp: mutex
 *  @return void
 */
void mutex_lock(mutex_t *mp){ 
	if( mp->status == MUTEX_DESTORY){
		lprintf("Lock has been destroyed already!");
		return;
	}
	while(1){
		//lprintf("before lock: mp->lock:%d", mp->lock);
		if(  atom_xchg(&mp->lock, LOCK) == UNLOCK){
		//	lprintf("Got lock! mp->lock:%d",mp->lock);
			break;
		}
		else{
		//	lprintf("Waiting for lock!");
			continue;
		}

	}
	return;
}	

/** @brief Unlock a mutex
 *  
 *  @param  mp: mutex
 *  @return void
 */
void mutex_unlock(mutex_t *mp){
	if( mp == NULL){
		lprintf("ERROR: Illegal mutex to unlock");
		return;
	}
	mp->lock = UNLOCK;
	return;
}

/** @brief Destroy a mutex
 *  
 *  @param  mp: mutex
 *  @return void
 */
void mutex_destroy(mutex_t *mp){
	while(1){
		if( mp->lock == UNLOCK){
			mp->status = MUTEX_DESTORY;
			lprintf("Destroy the mutex successfully!");
			return;
		}
		else
			continue;
	}
}




