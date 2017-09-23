/* resched.c - resched, resched_cntl */

#include <xinu.h>

#define INT_MAX 2147483647
 				
extern uint32 myclock;          /*Extra added by sbalekut             */
struct	defer	Defer;

/*------------------------------------------------------------------------
 *  resched  -  Reschedule processor to highest priority eligible process
 *------------------------------------------------------------------------
 */
void	resched(void)		/* Assumes interrupts are disabled	*/
{
	struct procent *ptold;	/* Ptr to table entry for old process	*/
	struct procent *ptnew;	/* Ptr to table entry for new process	*/

	/* If rescheduling is deferred, record attempt and return */

	if (Defer.ndefers > 0) {
		Defer.attempt = TRUE;
		return;
	}

	/* Point to process table entry for the current (old) process */

	ptold = &proctab[currpid];

	ptold->lasttime=myclock- (ptold->lasttime);
	
	kprintf("\n resched (), current process is %s and preeampt %d with priority %d", ptold->prname,preempt,ptold->prprio);
	
	kprintf("\n last time is %d \n", ptold->lasttime);
	if(currpid != NULLPROC)                                                  /*Extra added by sbalekut */
	{

	ptold->prprio=ptold->prprio+ ((ptold->lasttime * 100) / ptold->rate); /*Extra added by sbalekut */ 
	
	}

	
	  
	
	if (ptold->prstate == PR_CURR) {  /* Process remains eligible */
		if (ptold->prprio > firstkey(readylist)) {
			ptold->lasttime=myclock;
			return;

		}

		/* Old process will no longer remain current */

		ptold->prstate = PR_READY;
		
	
		if(currpid==NULLPROC)
			{
		insert(currpid, readylist, ptold->prprio);
			}


		else
			{


			 insert(currpid, readylist, INT_MAX-ptold->prprio);
			}
	}

	/* Force context switch to highest priority ready process */

	currpid = dequeue(readylist);
	ptnew = &proctab[currpid];
	kprintf("\n resched() : new process is %s and preempt value is %d at %d \n",ptnew->prname,preempt,myclock);
	ptnew->prstate = PR_CURR;
	preempt = QUANTUM;	
	ptnew->lasttime=myclock;
		/* Reset time slice for process	*/
	ctxsw(&ptold->prstkptr, &ptnew->prstkptr);

	/* Old process returns here when resumed */

	return;
}

/*------------------------------------------------------------------------
 *  resched_cntl  -  Control whether rescheduling is deferred or allowed
 *------------------------------------------------------------------------
 */
status	resched_cntl(		/* Assumes interrupts are disabled	*/
	  int32	defer		/* Either DEFER_START or DEFER_STOP	*/
	)
{
	switch (defer) {

	    case DEFER_START:	/* Handle a deferral request */

		if (Defer.ndefers++ == 0) {
			Defer.attempt = FALSE;
		}
		return OK;

	    case DEFER_STOP:	/* Handle end of deferral */
		if (Defer.ndefers <= 0) {
			return SYSERR;
		}
		if ( (--Defer.ndefers == 0) && Defer.attempt ) {
			resched();
		}
		return OK;

	    default:
		return SYSERR;
	}
}
