/** @file mutex_type.h
 *  @brief This file defines the type for mutexes.
 */

#ifndef _MUTEX_TYPE_H
#define _MUTEX_TYPE_H

/* Thread lock or not */
#define LOCK    0
#define UNLOCK  1

#define MUTEXT_INIT_SUCC  0
//#define MUTEXT_INIT_FAIL  1

#define MUTEX_EXIST       0
#define MUTEX_DESTORY     1

#define MUTEXT_DEFAULT_TID -1

typedef struct mutex {
  int lock;
  int tid;
  int status;
  int lock_flag;
  int lock_num;
} mutex_t;

void inline count_mutex_acquire(mutex_t *mp);
void inline count_mutex_release(mutex_t *mp);


#endif /* _MUTEX_TYPE_H */
