/** @file 
 *  @brief 
 *  @author 
 *  @author 
 *
 *  @bugs
 *
 */

#include <syscall_int.h>
.global yield

yield:
    push %esi
    movl 8(%esp), %esi 
    int $YIELD_INT
    pop %esi
    ret

