#include <assert.h>
#include <stdio.h>
#include <stddef.h>
#include <syscall.h>
#include <simics.h>
#include "rand.h"

#include "thread.h"
//#include "cond.h"
//#include "mutex.h"
//#include "thrgrp.h"

int shared;

void *child(void *param);

#define STACK_SIZE 3072

#define NTHREADS 30
#define SOMETIMES 4


int main()
{
    shared = 0;
  int k;
  lprintf ("IN main &k = %x", (int) &k);
  thr_init(STACK_SIZE);
  //mutex_t lock;
  //mutex_init(&lock);

  int t;
  for (t = 0; t < 3; ++t) {
    thr_create(child, (void *) NULL);
  }
  lprintf("main thread ends!");
  return 0;
}

/** @brief Declare that we have run, then twiddle thumbs. */
void *
child(void *param)
{
    int a[100];
    lprintf("child");
    lprintf ("a[0] address is %x",(int) &a[0]);
    lprintf ("a[2047] address is %x",(int) &a[99]);
    //mutex_t *lp = (mutex_t *)param;
    int i = 1;
    lprintf("%x",(int)&i);	
    //mutex_lock(lp);
    shared++;
    lprintf("shared: %d", shared);
    //mutex_unlock(lp);
    lprintf("unlock!");
    return NULL;
}
