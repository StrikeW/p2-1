/** @file 
 *  @brief 
 *  @author 
 *  @author 
 *
 *  @bugs
 *
 */

#include <syscall_int.h>
.global sleep

sleep:
    push %esi
    movl 8(%esp), %esi
    int $SLEEP_INT 
    pop %esi
    ret

