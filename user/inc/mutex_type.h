/** @file  mutex_type.h
 *  @brief mutex data structure and its
 *         associated functions
 *
 *  @author  Yuhang Jiang (yuhangj)
 *  @author  Subramanian Natarajan (snatara1)
 *
 *  @bugs No known bugs
 *
 */


#ifndef _MUTEX_TYPE_H
#define _MUTEX_TYPE_H

/* mutex lock unlock macros  */
#define LOCK    1
#define UNLOCK  0

/* mutex operation macros */
#define MUTEXT_INIT_SUCC  0
#define MUTEXT_INIT_FAIL  -1


/* Mutex existence macros*/
#define MUTEX_DESTORY     0
#define MUTEX_EXIST       1

#define MUTEXT_DEFAULT_TID -1

/* mutex data structure definition */

typedef struct mutex {
  int lock;
  int status;
} mutex_t;

/* Function prototypes */
int mutex_init(mutex_t *mp);
void mutex_lock(mutex_t *mp);
void mutex_unlock(mutex_t *mp);
void mutex_destroy(mutex_t *mp);



#endif /* _MUTEX_TYPE_H */
