#include <xinu.h>

devcall pipputc(struct dentry *devptr, char ch) {




struct pipe_t *pip_ptr;

pipid32 pip_id;

pip_id= devptr->dvminor;


if ( pip_id < 0 || pip_id >= MAXPIPES)
{


return SYSERR;

}



pip_ptr=&pipe_tables[pip_id];

wait(pip_ptr->write_sem);

pip_ptr->pipe_buffer[++pip_ptr->write_pos]=ch;

if(pip_ptr->write_pos +1  >= PIPE_SIZE ) {

pip_ptr->write_pos=-1;

}


signal(pip_ptr->read_sem);






    // LAB2: TODO
   return OK;
}

