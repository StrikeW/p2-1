/**
 * @file multitest.c
 *
 * @brief A crazy test of the thread implementation.
 *
 * This program keeps one copy of a bunch of thread-based
 * programs running at once.
 *
 * @author Alejandro Lince (alince)
 * @author Spencer Whitman (swhitman)
 *
 * @bug None known
 */

#include <syscall.h>
#include <simics.h>
#include <mutex.h>
#include <stdio.h>
#include <thread.h>
#include <sem.h>

#define TESTS 15

int main()
{
  int i;
  int count = 0;
  char * tests[] = {"agility_drill", "actual_wait","cat", "cvar_test", "cyclone", "excellent", "getpid_test1", "join_specific_test", \
		    "mutex_destroy_test","paraguay", "stack_test1","startle","switzerland","thr_exit_join","wild_test1"};

  char * ad[] = {"agility_drill",(char *)0};
  char * aw[] = {"actual_wait",(char *)0};
  char * cat[] = {"cat",(char *)0};
  char * ct[] = {"cvar_test",(char *)0};
  char * c[] = {"cyclone",(char *)0};
  char * ex[] = {"excellent",(char *)0};
  char * gp[] = {"gp",(char *)0};
  char * jst[] = {"join_specific_test",(char *)0};
  char * mdt[] = {"mutex_destroy_test",(char *)0};
  char * py[] = {"paraguay",(char *)0};
  char * sct1[] = {"stack_test1",(char *)0};
  char * st[] = {"startle",(char *)0};
  char * sw[] = {"switzerland",(char *)0};
  char * tej[] = {"thr_exit_join",(char *)0};
  char * wt[] = {"wild_test1",(char *)0};

  char ** argsvec[] = { ad,aw,cat,ct,c,ex,gp,jst,mdt,py,sct1,st,sw,tej,wt};
  
   while(1) {
    lprintf("-----------------Iteration: %d-----------------------\n",count++);
    for(i=0;i<TESTS;i++) {
      sleep(5);
      misbehave(-1);
      int tid = fork();
      
      if(tid == 0) {
	printf("execing %s\n",tests[i]);
	exec(tests[i],argsvec[i]);
	printf("test %s could not exec\n",tests[i]);
	return 0;
      }
      
    }
  }

}
