/** @file xchg.S
 *
 *  @brief Implementation of atomic xchg in assembly 
 *  
 *  @bug No known bug
 */

 .global atom_xchg

 atom_xchg:
 	movl 8(%esp),%eax  #LOCK = 1
 	movl 4(%esp),%ecx  #mp->lock
 	XCHG (%ecx), %eax  #eax = mp->lock
 	ret
