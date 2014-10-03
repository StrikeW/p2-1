/** @file  mutex.c
 *  @brief Implementation of the mutex library
 *
 *  The thread library uses the xchg 
 *  instruction to lock the mutex. 
 *  For threads who try to lock 
 *  an already locked mutex, they
 *  busy wait for lock to become
 *  available.
 *
 *  Since we expect Critical section
 *  to be executed in as short
 *  duration as possible,
 *  the busy wait cost should
 *  be negligible.
 *
 *
 *  @author  Yuhang Jiang (yuhangj)
 *  @author  Subramanian Natarajan (snatara1)
 *
 *  @bugs No known bugs
 *
 */

#include <syscall.h>
#include <assert.h>
#include <stdio.h>
#include "simics.h"
#include "mutex_type.h"

extern int atom_xchg (int *, int);




/** @brief  Function to initialize mutex
 *    
 *  mutex datastructure initialised to 
 *  default values. 
 *    
 *  @param  mp: mutex
 *  @return MUTEXT_INIT_SUCC on success 
 *          MUTEXT_INIT_FAIL on failure
 */

int
mutex_init (mutex_t * mp)
{
    if ( mp->status == MUTEX_EXIST ) {
	lprintf(" Mutex already initialised");
	return MUTEXT_INIT_SUCC;
    }
    if (mp == NULL)
    {
	lprintf ("ERROR: Illegal mutex to init!");
	return MUTEXT_INIT_FAIL;
    }
    mp->lock = UNLOCK;
    mp->status = MUTEX_EXIST;
    return MUTEXT_INIT_SUCC;
}



/** @brief Function to lock mutex
 *
 *  Using the atomic xchg instruction 
 *  to lock the mutex.
 *  If the mutex is already locked, 
 *  then wait for it to become 
 *  available.
 *
 *  @param  mp mutex to be locked
 *  @return void
 */
void
mutex_lock (mutex_t * mp)
{
    /* TODO: duplicate lock check */
    /* TODO: Remove lprintf */
    
	
    /* Check whether the mutex has been destroyed */
    if (mp->status == MUTEX_DESTORY)
    {
      lprintf ("Lock has been destroyed already!");
      return;
    }
    while (1)
    {
        /* Lock the mutex with the XCHG instruction */
	if (atom_xchg (&mp->lock, LOCK) == UNLOCK)
	{
	  break;
	}
	else
	{
	  /* Wait for unlock */
	  continue;
	}

    }
    return;
}

/** @brief Function to unlock the mutex
 *   Unlock mutex by setting lock
 *   member variable to UNLOCK
 *
 *  @param  mp: mutex to be unlocked
 *  @return void
 */

void
mutex_unlock (mutex_t * mp)
{
    /* TODO: duplicate unlock check */
    if (mp == NULL)
    {
	lprintf ("ERROR: Illegal mutex to unlock");
	return;
    }
    mp->lock = UNLOCK;
    return;
}

/** @brief Function to destroy the mutex
 *  
 *  @param  mp mutex to be destroyed
 *  @return void
 */

void
mutex_destroy (mutex_t * mp)
{
    if ( mp->status == LOCK) {
	lprintf ("Mutex is locked, Unlock to destroy");
	return;
    }    

    while (1)
    {
	if (mp->lock == UNLOCK)
	{
	    mp->status = MUTEX_DESTORY;
	    lprintf ("Destroy the mutex successfully!");
	    return;
	}
        else 
	{
	    continue;
	}
    }
}
