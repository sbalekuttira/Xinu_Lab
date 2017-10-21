#include <xinu.h>

status pipdisconnect(did32 devpipe) {

pipid32 pipe_id;

pipe_id=did32_to_pipid32(devpipe);

struct	pipe_t	*pip_ptr;		
	
intmask mask;	

mask = disable();

pip_ptr = &pipe_tables[pipe_id];

if(pipe_id<0 || pipe_id > MAXPIPES)
		{
	
		restore(mask);
		return SYSERR;
		}
		
		if(pip_ptr->state!=PIPE_CONNECTED && pip_ptr->state!=PIPE_WRITE_DISCONNECTED && pip_ptr->state!=PIPE_READ_DISCONNECTED) 
		{

		restore(mask);
		return SYSERR;
		}

			


	if(currpid!=pip_ptr->reader && currpid!=pip_ptr->writer)

		{
			restore(mask);
			return SYSERR;

		}

		if((pip_ptr->state==PIPE_READ_DISCONNECTED && pip_ptr->writer==currpid)||(pip_ptr->state==PIPE_WRITE_DISCONNECTED && pip_ptr->reader==currpid))
			{
		
			pip_ptr->state=PIPE_USED;
			pip_ptr->writer=-1;
			pip_ptr->reader=-1;
			semreset(pip_ptr->read_sem,0);
			semreset(pip_ptr->write_sem,PIPE_SIZE);	

			restore(mask);
			return OK;
			}

			
	if(currpid==pip_ptr->reader)
			{
			pip_ptr->state=PIPE_READ_DISCONNECTED;
			pip_ptr->reader=-1;
			}

			
	if(currpid==pip_ptr->writer)
			{
			pip_ptr->state=PIPE_WRITE_DISCONNECTED;
			pip_ptr->writer=-1;
			}

		


restore(mask);



    // LAB2: TODO
	return OK;
}
