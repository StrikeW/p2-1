#include <syscall.h>
#include <thread.h>
#include <stdlib.h>
#include <assert.h>
#include "simics.h"
#include "cond_type.h"
#include "sem_type.h"


/** @brief Semaphores initialization
 *  
 *  @param  sem: semaphores 
 *          count: set number
 *  @return 0: success
 */
int sem_init( sem_t *sem, int count ){
	assert(sem != NULL);
	sem->count = count;
	sem->status = SEM_EXIST;

	if(mutex_init(&sem->sem_mutex) != 0)
		return MUTEX_INIT_ERROR;

	if(cond_init(&sem->sem_cond) != 0)
		return CONDV_INIT_ERROT;

	return SEM_INIT_SUCC;
}


/** @brief Destroy semaphores 
 *  
 *  @param  sem: semaphores 
 *  @return void
 */
void sem_destroy( sem_t *sem ){
// If use while loop here, it may grasp and release lock again
// and again. If there are some threads are waitting, it has
// the possiblity that these threads won't get the lock 
// forever. So, if there are some threads on the list, yield
// to other threads to run. Then back to destroy the sem.

	/* Cannot destroy a sem which has already destroyed */
	assert(sem->status == SEM_EXIST);

	while(1){
		mutex_lock(&sem->sem_mutex);

		/* Some threads are on the waitting list */
		if(sem->count < 0){
			lprintf("Cannot destroy the semaphore!");
			mutex_unlock(&sem->sem_mutex);
			yield(-1);
		}
		/* We can destroy the sem now */
		if(sem->count >= 0){
			sem->status = SEM_DESTROY;
			mutex_unlock(&sem->sem_mutex);
			mutex_destroy(&sem->sem_mutex);
			cond_destroy(&sem->sem_cond);
			break;
		}
	}
	return;
}

/** @brief Semaphores wait
 *  
 *  @param  sem: semaphores 
 *  @return void
 */
void sem_wait( sem_t *sem ){
	/* If already destroyed, return */
	if(sem->status == SEM_DESTROY){
		lprintf("sem has destroyed already!");
		return;
	}

	mutex_lock(&sem->sem_mutex);

	sem->count--;
	if(sem->count < 0){
		lprintf("sem->count < 0 !");
		cond_wait(&sem->sem_cond, &sem->sem_mutex);
	}

	mutex_unlock(&sem->sem_mutex);

	return;
}

/** @brief Release one thread from the waitting list
 *  
 *  @param  sem: semaphores 
 *  @return void
 */
void sem_signal( sem_t *sem ){
	
	mutex_lock(&sem->sem_mutex);

	sem->count++;
	cond_signal(&sem->sem_cond);

	mutex_unlock(&sem->sem_mutex);

	return;
}