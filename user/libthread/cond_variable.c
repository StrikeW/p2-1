#include <syscall.h>
#include <thread.h>
#include <stdlib.h>
#include <assert.h>
#include "variable_queue.h"
#include "cond_type.h"
#include "spin_lock.h"
#include "mutex_type.h"

extern int atom_xchg(int*, int);
// int cond_init( cond_t *cv );
// void cond_destroy( cond_t *cv );
// void cond_wait( cond_t *cv, mutex_t *mp );
// void cond_signal( cond_t *cv );
// void cond_broadcast( cond_t *cv );
//Q_NEW_HEAD(list_t, cond);

int cond_init(cond_t *cv) {
	assert(cv != NULL);
	
	spin_lock_init(&cv->spinlock);
	
	cv->tid = DEFAULT_TID;
	cv->status = COND_EXIST;
	// Q_NEW_HEAD(waiting_list_t, cond);
	// waiting_list_t waiting_list;
	Q_INIT_HEAD(&waiting_list);
	return 0;
}

void cond_destroy(cond_t *cv)
{
    assert(cv != NULL);
    while(1){
    	spin_lock_request(&cv->spinlock);
    	/* If there are threads on the waitting list */
 		if(waiting_list.size > 0){
 			/* Unlock the spinlock and waitting for size = 0 */
 			spin_lock_release(&cv->spinlock);
 			continue;
 		}
    	if(waiting_list.size == 0){
    		/* Set the cond_variable's status as destroyed */
    		cv->status = COND_DESTORY;
    		/* Unlock the spinlock and break */
    		spin_lock_release(&cv->spinlock);
    		break;
    	}
    }
    return;
}


void cond_wait(cond_t *cv, mutex_t *mp){
	int status = 0;

	assert(cv != NULL);
	assert(mp != NULL);
	/* If cond_variable has been destroyed, do nothing and return */
	if(cv->status == COND_DESTORY){
		lprintf("The cond_variable has been destroyed already");
		return;
	}
	
	//allocate mem for the thread want to grab resource
	cond_t *node_ptr = malloc(sizeof(cond_t));

//********** NOt sure to put spin_lock at which place!!!!!!!

	/* Lock the queue to store the thread info */
	spin_lock_request(&cv->spinlock);

	/* Initialize the struct to store the thread's it and 
	 * prepare to insert */
	Q_INIT_ELEM(node_ptr, cond_link);
	node_ptr->tid = gettid();

//********** May be here???????!!!!!!!

	/* Lock the queue to store the thread info */
	//spin_lock_request(&cv->spinlock);

	/* Insert into the tail of the waitting list */
	Q_INSERT_TAIL(&waiting_list, node_ptr, cond_link);

	/* Release the mutex mp passed in */
	mutex_unlock(mp);

	/* Release the lock of the queue */
	spin_lock_release(&cv->spinlock);

	/* Deschedule the thread who has inserted into the queue */
	deschedule(&status);

	/* Lock the passed mutex mp again */
	mutex_lock(mp);

	return;
}

void cond_signal(cond_t *cv){
	assert(cv != NULL);

	cond_t *node_ptr = NULL;
	int tid = DEFAULT_TID;

	/* Lock the queue */
	spin_lock_request(&cv->spinlock);

	/* Get the head node(thread) in the queue */
	node_ptr = Q_GET_FRONT(&waiting_list);

	/* If there are no thread in the queue, unlock and return */
	if(node_ptr == NULL){
		lprintf("No blocked threads!");
		spin_lock_release(&cv->spinlock);
		return;
	}/* If get, remove the first node from the queue */
	else{
		/* Q_REMOVE only delete the info of prev and next 
		 * which we won't use them anymore */
		Q_REMOVE(&waiting_list, node_ptr, cond_link);
	}

	/* Got one waitting thread from the queue, unlock the queue */
	tid = node_ptr->tid;
	spin_lock_release(&cv->spinlock);

	/* Make that thread runnable */
	while(make_runnable(tid) < 0)
		yield(tid);

	/* Free */
	free(node_ptr);

	return;
}

void cond_broadcast(cond_t *cv){
	cond_t *node_ptr = NULL;
	assert(cv != NULL);

	/* Lock the queue */
	spin_lock_request(&cv->spinlock);

	while(1){
		/* Wake up all the thread in the queue */
		node_ptr = Q_GET_FRONT(&waiting_list);
		if(node_ptr == NULL){
			lprintf("In cond_broadcast: no threads!");
			break;
		}/* Remove the node from the waitting list */
		else{
			Q_REMOVE(&waiting_list, node_ptr, cond_link);
			/* Make the thread runnable */
			make_runnable(node_ptr->tid);
		}
	}

	/* Unlock the thread */
	spin_lock_release(&cv->spinlock);

	return;
}










