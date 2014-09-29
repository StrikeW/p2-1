/** @file mutex_type.h
 *  @brief This file defines the type for mutexes.
 */

#ifndef _MUTEX_TYPE_H
#define _MUTEX_TYPE_H

/* Thread lock or not */
#define LOCK    1
#define UNLOCK  0

#define MUTEXT_INIT_SUCC  0
#define MUTEXT_INIT_FAIL  -1

#define MUTEX_DESTORY     0
#define MUTEX_EXIST       1

#define MUTEXT_DEFAULT_TID -1

typedef struct mutex {
  int lock;
  int status;
} mutex_t;

int mutex_init(mutex_t *mp);
void mutex_lock(mutex_t *mp);
void mutex_unlock(mutex_t *mp);
void mutex_destroy(mutex_t *mp);



#endif /* _MUTEX_TYPE_H */
