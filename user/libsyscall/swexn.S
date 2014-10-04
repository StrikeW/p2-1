/** @file 
 *  @brief 
 *  @author 
 *  @author 
 *
 *  @bugs
 *
 */

#include <syscall_int.h>
.global swexn

swexn:
    push %esi
    leal 8(%esp), %esi
    int $SWEXN_INT
    pop %esi
    ret 

