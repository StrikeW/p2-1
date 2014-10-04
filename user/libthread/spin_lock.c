/** @file   spin_lock.c 
 *  @brief  File containing implementation of spinlock functions
 *
 *  @author  Yuhang Jiang
 *  @author  Subramanian Natarajan
 *
 *  @bugs No known bugs
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "thr_internals.h"
#include "simics.h"
#include "spin_lock.h"

extern int atom_xchg(int*, int);

/** @brief  Function to initialize spinlock
 *    
 *  spinlock datastructure initialised to default values. 
 *    
 *  @param  spinlock data structure
 *  @return void
 */

void spin_lock_init(spinlock_t *spinlock){
    assert(spinlock != NULL);
    spinlock->lock = SPIN_UNLOCK;
}

/** @brief  Function to request spin lock
 *     
 *  Spin until you get the spin lock to get the critical section 
 *
 *  @param  spinlock datastructure
 *  @return  void
 */
void spin_lock_request(spinlock_t *spinlock){
    int status;

    assert(spinlock != NULL);

    while(1){
        status = atom_xchg(&spinlock->lock, SPIN_LOCK);
        if(status == SPIN_UNLOCK){
            break;
        }else{
            continue;
        }
    }
}

/** @brief  Function to release spin lock
 *     
 *   Release spin lock so that others can enter critical section
 *
 *  @param  spinlock datastructure
 *  @return  void
 */
void spin_lock_release(spinlock_t *spinlock){
    assert(spinlock != NULL);
    spinlock->lock = SPIN_UNLOCK;
}
