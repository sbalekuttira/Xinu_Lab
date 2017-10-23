#include <xinu.h>

devcall pipgetc(struct dentry *devptr) {
  
struct pipe_t *pip_ptr;
pipid32 pip_id;
char ch; 

pip_id= devptr->dvminor;


if ( currpid== NULLPROC ||  pip_id < 0 || pip_id >= MAXPIPES)
{


return SYSERR;

}



pip_ptr=&pipe_tables[pip_id];

wait(pip_ptr->read_sem);

ch=pip_ptr->pipe_buffer[++pip_ptr->read_pos];

if(pip_ptr->read_pos +1  >= PIPE_SIZE ) {

pip_ptr->read_pos=-1;

}


signal(pip_ptr->write_sem);






    // LAB2: TODO
   return (devcall) ch;
}


  

