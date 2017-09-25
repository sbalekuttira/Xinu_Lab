/* ready.c - ready */

#include <xinu.h>
#define	INT_MAX 2147483647
qid16 readylist;			/* Index of ready list		*/

/*------------------------------------------------------------------------
 *  ready  -  Make a process eligible for CPU service
 *------------------------------------------------------------------------
 */
status	ready(
	  pid32		pid		/* ID of process to make ready	*/
	)
{
	register struct procent *prptr;

	if (isbadpid(pid)) {
		return SYSERR;
	}

	/* Set process state to indicate ready and add to ready list */

	prptr = &proctab[pid];
	prptr->prstate = PR_READY;
	if(pid==NULLPROC) {
	insert(pid,readylist,prptr->prprio);
	}
	else if(prptr->group==0)
	{
	insert(pid,readylist,INT_MAX - prptr->prprio);
	}
	 else if(prptr->group==1)
        {
        insert(pid,readylist,prptr->prprio);
        }

	

	resched();

	return OK;
}
