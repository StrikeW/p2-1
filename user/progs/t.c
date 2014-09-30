#include <assert.h>
#include <stdio.h>
#include <stddef.h>
#include <syscall.h>
#include <simics.h>
#include "rand.h"

#include "thread.h"
//#include "cond.h"
#include "mutex_type.h"
//#include "thrgrp.h"

int shared;
mutex_t *lock;
void *child(void *param);

#define STACK_SIZE 3072

#define NTHREADS 1 
#define SOMETIMES 4


int main()
{
    shared = 0;
    int k;
    lprintf ("IN main &k = %x", (int) &k);
    thr_init(STACK_SIZE);
    lock = (mutex_t*)malloc(sizeof(mutex_t));
    mutex_init(lock);

    int t;
    for (t = 0; t < NTHREADS; ++t) {
      thr_create(child, (void *) NULL);
    }
    lprintf("main thread ends!");
    return 0;
}

/** @brief Declare that we have run, then twiddle thumbs. */
void *
child(void *param)
{
    mutex_lock(lock);
    
    shared++;
    lprintf("In child [%d] get the lock", gettid());
    lprintf("shared: %d",  shared);

    mutex_unlock(lock);
    lprintf("unlock!");
    return NULL;
}
