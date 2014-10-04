/** @file  sem.c
 *  @brief semaphore library implementation 
 *
 *  @author  Yuhang Jiang (yuhangj)
 *  @author  Subramanian Natarajan (snatara1)
 *
 *  @bugs No known bugs
 */

#include <syscall.h>
#include <thread.h>
#include <stdlib.h>
#include <assert.h>

#include "simics.h"
#include "cond_type.h"
#include "sem_type.h"

/** @brief Function to initialize semaphore
 *  
 *  Initialise a semaphore and set its condition variable and 
 *  mutex to default values.
 *
 *  @param  sem: semaphore that is to be initialized
 *  @param  count: count number of semaphore
 *  @return SEM_INIT_SUCC on  success 
 *          MUTEX_INIT_ERROR if mutex init failed
 *          CONDV_INIT_ERROT if condition variable init failed 
 */      
int
sem_init (sem_t * sem, int count)
{
    assert (sem != NULL);
    sem->count = count;
    sem->status = SEM_EXIST;

    if (mutex_init (&sem->sem_mutex) != 0)
	   return MUTEX_INIT_ERROR;

    if (cond_init (&sem->sem_cond) != 0)
	   return CONDV_INIT_ERROT;

    return SEM_INIT_SUCC;
}


/** @brief Function to destroy semaphore
 *  
 *  Destroy the mutex if and only if are no threads waiting on 
 *  the semaphore
 *
 *  @param  sem: semaphore that needs to be destroyed 
 *  @return void
 */
void
sem_destroy (sem_t * sem)
{
// If use while loop here, it may grasp and release lock again
// and again. If there are some threads are waitting, it has
// the possiblity that these threads won't get the lock 
// forever. So, if there are some threads on the list, yield
// to other threads to run. Then back to destroy the sem.

   /* Cannot destroy a sem which has already destroyed */
   assert (sem->status == SEM_EXIST);

   while (1)
   {
	mutex_lock (&sem->sem_mutex);

	/* Some threads are on the waiting list */
	if (sem->count < 0)
	{
	    lprintf ("Cannot destroy the semaphore!");
	    mutex_unlock (&sem->sem_mutex);
	    yield (-1);
	}
	/* We can destroy the sem now */
	if (sem->count >= 0)
	{
	    sem->status = SEM_DESTROY;
	    mutex_unlock (&sem->sem_mutex);
	    mutex_destroy (&sem->sem_mutex);
	    cond_destroy (&sem->sem_cond);
	    break;
	}
    }
    return;
}

/** @brief Function to wait on the semaphore
 *  
 *  If the semaphore count value is equal to or less than zero 
 *  then wait for the count to become positive.
 *
 *  @param  sem semaphore on which wait is invoked 
 *  @return void
 */
void
sem_wait (sem_t * sem)
{
    /* If already destroyed, return */
    if (sem->status == SEM_DESTROY) {
        lprintf ("sem has destroyed already!");
        return;
    }

    mutex_lock (&sem->sem_mutex);

    while (sem->count <= 0) {
	   cond_wait (&sem->sem_cond, &sem->sem_mutex);
    }

    sem->count--;
    mutex_unlock (&sem->sem_mutex);

    return;
}

/** @brief Function to wake thread waiting on the semaphore and signal
 *
 *  Increment the count and let everyone know that it is available.
 *
 *  @param  sem semaphore on which the function is invoked
 *  @return void
 */
void
sem_signal (sem_t * sem)
{
    mutex_lock (&sem->sem_mutex);
    
    sem->count++;
    cond_signal (&sem->sem_cond);

    mutex_unlock (&sem->sem_mutex);

    return;
}
