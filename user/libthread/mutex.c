/** @file mutex.c
 *
 *  @brief Implementation of mutex
 *  
 *  @bug No known bug
 */
#include <syscall.h>
#include "mutex_type.h"

extern int atom_xchg(int*, int);
/** @brief Mutex intialization
 *  
 *  @param  mp: mutex
 *  @return 0: success
 */
int mutex_init(mutex_t *mp){
	mp->lock = UNLOCK;
	mp->tid = MUTEXT_DEFAULT_TID;
	mp->status = MUTEX_EXIST;
	mp->lock_flag = UNLOCK;
	mp->lock_num = 0;
	return MUTEXT_INIT_SUCC;
}

/** @brief Lock a mutex
 *  
 *  @param  mp: mutex
 *  @return void
 */
void mutex_lock(mutex_t *mp){

	count_mutex_acquire(mp);

	if( mp->status == MUTEX_DESTORY)	
		return;

	while( atom_xchg(&mp->lock, LOCK)){
		yield(mp->tid);
	}
	mp->tid = gettid();
	return;
}	

/** @brief Unlock a mutex
 *  
 *  @param  mp: mutex
 *  @return void
 */
void mutex_unlock(mutex_t *mp){
	mp->tid = MUTEXT_DEFAULT_TID;

	count_mutex_release(mp);

	mp->lock = UNLOCK;
	return;
}

/** @brief Destroy a mutex
 *  
 *  @param  mp: mutex
 *  @return void
 */
void mutex_destroy(mutex_t *mp){
	mp->status = MUTEX_DESTORY;

	// wait until no mutex is needed 
	// Not sure this is ok
	while(1){
		while(atom_xchg(&mp->lock_flag, LOCK) != UNLOCK)
			yield(-1);
		if(mp->lock_flag != 0){
			mp->lock_flag = UNLOCK;
			yield(mp->tid);
		}else{
			mp->lock_flag = UNLOCK;
			break;
		}
	}

}
/** @brief Count how many threads want to acquire mutex_lock
 *  
 *  @param  mp: mutex
 *  @return void
 */
void inline count_mutex_acquire(mutex_t *mp){
	while( atom_xchg(&mp->lock_flag, LOCK) != UNLOCK){
		yield(-1);
	}
	mp->lock_num++;
	mp->lock_flag = UNLOCK;
}

/** @brief Count how many threads want to release the mutex_lock
 *  
 *  @param  mp: mutex
 *  @return void
 */
void inline count_mutex_release(mutex_t *mp){
	while( atom_xchg(&mp->lock_flag, LOCK) != UNLOCK){
		yield(-1);
	}
	mp->lock_num--;
	mp->lock_flag = UNLOCK;
}




