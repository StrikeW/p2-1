/** @file 
 *  @brief 
 *  @author 
 *  @author 
 *
 *  @author  Yuhang Jiang
 *  @author  Subramanian Natarajan
 *
 *  @bugs
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "simics.h"
#include "spin_lock.h"

extern int atom_xchg(int*, int);

void spin_lock_init(spinlock_t *spinlock){
    assert(spinlock != NULL);
    spinlock->lock = SPIN_UNLOCK;
}

void spin_lock_request(spinlock_t *spinlock){
    int status;

    assert(spinlock != NULL);

    while(1){
        status = atom_xchg(&spinlock->lock, SPIN_LOCK);
        if(status == SPIN_UNLOCK){
            //lprintf("spin_lock locked");
            break;
        }else{
            //lprintf("Acquiring the spin_lock");
            continue;
        }
    }
}

void spin_lock_release(spinlock_t *spinlock){
    assert(spinlock != NULL);
    //lprintf("Release spinlock");
    spinlock->lock = SPIN_UNLOCK;
}
