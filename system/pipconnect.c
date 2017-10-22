#include <xinu.h>

status pipconnect(did32 devpipe , pid32 writer, pid32 reader) {

pipid32 pipe_id;
pipe_id=did32_to_pipid32(devpipe);



intmask	mask;	
struct	pipe_t *pip_ptr;		

int32	i;

mask = disable();




if(pipe_id < 0 ||pipe_id >= MAXPIPES)
		{

			restore(mask);
			return SYSERR;			
		}


	if(reader==writer||isbadpid(reader)||isbadpid(writer)  )
		{
			
			restore(mask);
			return SYSERR;			
		}

pip_ptr = &pipe_tables[pipe_id];

	
	if( pip_ptr->state!=PIPE_USED )
		{
			
			restore(mask);
			return SYSERR;			
		}
	

pip_ptr->reader=reader;
pip_ptr->writer=writer;

pip_ptr->read_pos=-1;
pip_ptr->write_pos=-1;

for(i=0;i<PIPE_SIZE;i++)
	{
	pip_ptr->pipe_buffer[i]=NULL;
	}


semreset(pip_ptr->read_sem,0);
semreset(pip_ptr->write_sem,PIPE_SIZE);
	

pip_ptr->state=PIPE_CONNECTED;



restore(mask);
    // LAB2: TODO
	return OK;
}
