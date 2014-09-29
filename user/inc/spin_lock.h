#ifndef __SPINLOCK_H__
#define __SPINLOCK_H__

#define SPIN_LOCK     1
#define SPIN_UNLOCK   0

typedef struct {
	int lock;
} spinlock_t;

void spin_lock_init(spinlock_t *spinlock);
void spin_lock_request(spinlock_t *spinlock);
void spin_lock_release(spinlock_t *spinlock);

#endif