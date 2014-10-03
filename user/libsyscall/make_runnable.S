/** @file 
 *  @brief 
 *  @author 
 *  @author 
 *
 *  @bugs
 *
 */


#include <syscall_int.h>
.global make_runnable

make_runnable:
    push %esi
    movl 8(%esp), %esi
    int $MAKE_RUNNABLE_INT
    pop %esi
    ret

