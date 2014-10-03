/** @file  cond_variable.c
 *  @brief contains functions and code 
 *  for condition variable implementation 
 *
 *
 *  @author  Yuhang Jiang (yuhangj)
 *  @author  Subramanian Natarajan (snatara1)
 *
 *
 *  @bugs No known bugs
 *
 */

#include <syscall.h>
#include <thread.h>
#include <stdlib.h>
#include <assert.h>

#include "variable_queue.h"
#include "cond_type.h"
#include "spin_lock.h"
#include "mutex_type.h"




/** @brief Function to initialise the condition variable
 *  initialise the condition variables to defaut values 
 *
 *  @param cv condition variable that needs to be initialised
 *  @return 1 on success , -1 on failure
 */
int
cond_init (cond_t * cv)
{
    assert (cv != NULL);

    if ( cv->status == COND_EXIST) {
        lprintf("Condition variable already exists ");
	return COND_VAR_SUCC;
    }
    
    spin_lock_init (&cv->spinlock);
    cv->tid = DEFAULT_TID;
    cv->status = COND_EXIST;

    Q_INIT_HEAD (&cv->waiting_list);
    return COND_VAR_SUCC;
}


/** @brief Function to destroy the condition variable
 *  
 *  Destroy the condition variable if and only if there
 *  are no threads in the waiting list 
 *  of the condition variable.
 *   
 *
 *  @param cv condition variable that needs to be destroyed
 *  @return  void
 */
void
cond_destroy (cond_t * cv)
{
    assert (cv != NULL);
    while (1)
    {
	spin_lock_request (&cv->spinlock);
        /* If there are threads on the waiting list */
	if (cv->waiting_list.size > 0)
	{
	    /* Unlock the spinlock and waiting for size = 0 */
	    spin_lock_release (&cv->spinlock);
	    yield (-1);
	}
	if (cv->waiting_list.size == 0)
	{
	   /* Set the cond_variable's status as destroyed */
	   cv->status = COND_DESTORY;
	   /* Unlock the spinlock and break */
	   spin_lock_release (&cv->spinlock);
	   break;
	}
    }
  return;
}


/** @brief Function that implements the wait for the condition variable
 *  
 *  Put the thread in the waiting list of the condition 
 *  variable and block the thread until it receives
 *  a signal to wake up.
 * 
 *
 *  @param cv condition variable that is waited on
 *  @param mp the mutex variable be acquired when signalled
 *  @return void 
 */
void
cond_wait (cond_t * cv, mutex_t * mp)
{
    int status = 0;
    assert (cv != NULL);
    assert (mp != NULL);

    /* If cond_variable has been destroyed, do nothing and return */
    if (cv->status == COND_DESTORY)
    {
        lprintf ("The cond_variable has been destroyed already");
        return;
    }

    /* Allocate memory for condition variable */
    cond_t *node_ptr = malloc (sizeof (cond_t));

    /* Initialize the struct to store the thread's it and 
    * prepare to insert */
    Q_INIT_ELEM (node_ptr, cond_link);
    node_ptr->tid = gettid ();

    /* Lock the queue to store the thread info */
    spin_lock_request (&cv->spinlock);

    /* Insert into the tail of the waiting list */
    Q_INSERT_TAIL (&cv->waiting_list, node_ptr, cond_link);

    /* Release the mutex mp passed in */
    mutex_unlock (mp);

    /* Release the lock of the queue */
    spin_lock_release (&cv->spinlock);


    /* Deschedule the thread who has inserted into the queue */
    deschedule (&status);

   /* Lock the passed mutex mp again */
   mutex_lock (mp);
   return;
}

/** @brief Function to wake up a thread waiting on the conditon
 *         variable
 *   
 *   Wake the first thread in the waiting list 
 *   of the condition variable.
 *
 *  @param *cv the cond variable we are signaling 
 *  @return void 
 */
void
cond_signal (cond_t * cv)
{
    assert (cv != NULL);

    cond_t *node_ptr = NULL;
    int tid = DEFAULT_TID;

    /* Lock the queue */
    spin_lock_request (&cv->spinlock);

    /* Get the head node(thread) in the queue */
    node_ptr = Q_GET_FRONT (&cv->waiting_list);

    /* If there are no thread in the queue, unlock and return */
    if (node_ptr == NULL)
    {
	lprintf ("No blocked threads!");
	spin_lock_release (&cv->spinlock);
	return;
    }		
    else
    {
    	/* If get, remove the first node from the queue */
	Q_REMOVE (&cv->waiting_list, node_ptr, cond_link);
    }

    /* Got one waitting thread from the queue, unlock the queue */
    tid = node_ptr->tid;

    spin_lock_release (&cv->spinlock);

    /* Make that thread runnable */
    while (make_runnable (tid) < 0)
    {
	lprintf ("ALREADY RUNNING!!!!!!!!!!!!!!!!!!!!!!!!!");
	yield (tid);
    }

    /* Free */
    free (node_ptr);

    return;
}

/* @brief Function to awaken all threads waiting on the
 * condition variable
 *
 * Broadcast signal to wake all threads in the waiting list.
 *
 * @param *cv the cond variable that is being broadcasted
 * @return Void.
 * 
 */
void
cond_broadcast (cond_t * cv)
{
  cond_t *node_ptr = NULL;
  assert (cv != NULL);

  /* Lock the queue */
  spin_lock_request (&cv->spinlock);
  while (1)
    {
      /* Wake up all the thread in the queue */
      node_ptr = Q_GET_FRONT (&cv->waiting_list);
      if (node_ptr == NULL)
	{
	  lprintf ("In cond_broadcast: no threads!");
	  break;
	}		
      else
	{
          /* Remove node from waiting list */
	  Q_REMOVE (&cv->waiting_list, node_ptr, cond_link);
	  /* Make the thread runnable */
	  make_runnable (node_ptr->tid);
	  /* After move the node, free space */
	  free (node_ptr);
	}
    }

  /* Unlock the thread */
  spin_lock_release (&cv->spinlock);

  return;
}
