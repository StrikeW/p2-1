/** @file 
 *  @brief 
 *  @author 
 *  @author 
 *
 *  @bugs
 *
 */

#include <syscall_int.h>
.global task_vanish

task_vanish:
    push %esi
    movl 8(%esp), %esi 
    int $TASK_VANISH_INT
    pop %esi
    ret

