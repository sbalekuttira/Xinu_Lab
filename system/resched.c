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
	/* for TS sched added by SBALEKUT */
	ptold = &proctab[currpid];
	kprintf("\n\n resched(): old process is %s of group %d with priority %d and quantum %d at time %d \n", ptold->prname,ptold->group,\
			ptold->prprio,preempt,myclock);

	if (ptold->group==1 && preempt==0 )
	{
		
			if(ptold->prprio>= 0 && ptold-> prprio <= 9) 
				{		
					ptold->tstrack=200;
					ptold->prprio=0;

				}
	
				else if (ptold->prprio == 10)
	
				{

					ptold->tstrack=200;
					ptold->prprio=0;


				}
				


				else if( ptold->prprio >=11 && ptold->prprio <20 )
				{	

					ptold->tstrack=160;
					ptold->prprio=ptold->prprio-10;

				}	

				 else if( ptold->prprio >=20 && ptold->prprio <30 )              
      				  {
                
               				 ptold->tstrack=120;
                			ptold->prprio=ptold->prprio-10;

			         }
				 else if( ptold->prprio >=30 && ptold->prprio <40 )              
       				 {
                
               				 ptold->tstrack=80;
               				 ptold->prprio=ptold->prprio-10;

       				 }
				 else if( ptold->prprio >=40 && ptold->prprio <59 )              
       				 {
                
              				  ptold->tstrack=40;
               				 ptold->prprio=ptold->prprio-10;

       				 }

	
				else if (ptold->prprio == 59)

        			{

              				  ptold->tstrack=20;
               				 ptold->prprio=ptold->prprio-10;


       				 }

							//CPU bound added by SBALEKUT
	}


	else if(ptold->group==1 && preempt > 0)			//IO BOUND Added by SBALEKUT
		{	kprintf("Inside IO BLock for process %s \n\n",ptold->prname); 
			if(ptold->prprio>= 0 && ptold-> prprio < 10)
       				 {
                			ptold->tstrack=200;
                			ptold->prprio=50;

        			}

       			 else if (ptold->prprio >= 10 && ptold->prprio <20)

        			{

                			ptold->tstrack=160;
               				 ptold->prprio=51;


       				 }



       			 else if( ptold->prprio >=20 && ptold->prprio <30 )
        			{

               				 ptold->tstrack=120;
               				 ptold->prprio=52;

        			}

										//IO bound dispatch table added by SBALEKUT
   			 else if (ptold->prprio >= 30 && ptold->prprio <40)

       				 {

               				 ptold->tstrack=80;
               				 ptold->prprio=54;


       				 }



       			 else if( ptold->prprio >=40 && ptold->prprio <45 )
       				 {

               				 ptold->tstrack=40;
               				 ptold->prprio=55;

       				 }
	   
			 else if (ptold->prprio == 45 )

       				 {

               				 ptold->tstrack=40;
              				  ptold->prprio=56;


       				 }



       			 else if( ptold->prprio ==46 )
       				 {

               				 ptold->tstrack=40;
               				 ptold->prprio=57;

       				 }

   			 else if (ptold->prprio >= 47 && ptold->prprio <59)

       				 {

               				 ptold->tstrack=40;
                			ptold->prprio=58;


       				 }



       			 else
       				 {

               				 ptold->tstrack=20;
               				 ptold->prprio=59;

        			}

		}




	ptold->lasttime=myclock- (ptold->lasttime);
	
//	kprintf("\n resched (), current process is %s and preeampt %d with priority %d \n", ptold->prname,preempt,ptold->prprio);
	
//	kprintf("\n last time is %d \n", ptold->lasttime);
	if(currpid != NULLPROC && ptold->group!=1)                                                  /*Extra added by sbalekut */
	{

	ptold->prprio=ptold->prprio+ ((ptold->lasttime * 100) / ptold->rate); /*Extra added by sbalekut */ 
	
	}

	
	  
	
	if (ptold->prstate == PR_CURR) {  /* Process remains eligible */
		if (ptold->prprio > firstkey(readylist)) {
			
			 if(ptold->group==1)
           		     {

                        preempt=ptold->tstrack;

               			 }
       			 else
                		{

                       			 preempt=QUANTUM;
					ptold->lasttime=myclock;
				}
		
			kprintf("Resched() : new process is %s of group %d with priority %d and quantum %d at time %d \n",\
				ptold->prname,ptold->group,ptold->prprio,preempt,myclock);
			return;

		}

		/* Old process will no longer remain current */

		ptold->prstate = PR_READY;
		
	
		if(currpid==NULLPROC)
			{
		insert(currpid, readylist, ptold->prprio);
			}


		else if(ptold->group==0)
			{


			 insert(currpid, readylist, INT_MAX-ptold->prprio);
			}
		else if (ptold->group==1)
			{

			insert(currpid, readylist, ptold->prprio);


			}
	}

	/* Force context switch to highest priority ready process */

	currpid = dequeue(readylist);
	ptnew = &proctab[currpid];
//	kprintf("\n resched() : new process is %s and preempt value is %d at %d \n",ptnew->prname,preempt,myclock);
	ptnew->prstate = PR_CURR;
	//	preempt = QUANTUM;

	if(ptnew->group==1)
		{

			preempt=ptnew->tstrack;

		}	
	else
		{

			preempt=QUANTUM;


		}
       kprintf("\n resched() : new process is %s of group %d with priority %d and QUANTUM %d at time %d \n\n \n", \
			ptnew->prname,ptnew->group,ptnew->prprio,preempt,myclock);
/* For was blocekd added by SBALEKut 
 if((ptnew->tlastsched==0 || ptnew->wasblocked==TRUE) && currpid!=NULLPROC && ptnew->group==0)
{ 
if(ptnew->prprio >= myclock)
	ptnew->prprio=ptnew->prprio;

else
	ptnew->prprio=myclock;

ptnew->blockingstate=false;

}

*/
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
