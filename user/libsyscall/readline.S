/** @file 
 *  @brief 
 *  @author 
 *  @author 
 *
 *  @bugs
 *
 */

#include <syscall_int.h>
.global readline

readline:
    push %esi
    leal 8(%esp), %esi
    int $READLINE_INT
    pop %esi
    ret

