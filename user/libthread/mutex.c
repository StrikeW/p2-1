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
		if( atom_xchg(&mp->lock, LOCK) == 0)
			break;
		else
			continue;
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

	// wait until no mutex is needed 
	// Not sure this is ok
	// while(1){
	// 	while(atom_xchg(&mp->lock_flag, LOCK) != UNLOCK)
	// 		yield(-1);
	// 	if( mp->lock_flag != 0){
	// 		mp->lock_flag = UNLOCK;
	// 		yield(mp->tid);
	// 	}else{
	// 		mp->lock_flag = UNLOCK;
	// 		break;
	// 	}
	// }

}
/** @brief Count how many threads want to acquire mutex_lock
 *  
 *  @param  mp: mutex
 *  @return void
 */
// void inline count_mutex_acquire(mutex_t *mp){
// 	while( atom_xchg(&mp->lock_flag, LOCK) != UNLOCK){
// 		yield(-1);
// 	}
// 	mp->lock_num++;
// 	mp->lock_flag = UNLOCK;
// }

/** @brief Count how many threads want to release the mutex_lock
 *  
 *  @param  mp: mutex
 *  @return void
 */
// void inline count_mutex_release(mutex_t *mp){
// 	while( atom_xchg(&mp->lock_flag, LOCK) != UNLOCK){
// 		yield(-1);
// 	}
// 	mp->lock_num--;
// 	mp->lock_flag = UNLOCK;
// }




