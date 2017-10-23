#include <xinu.h>

uint32 pipread(struct dentry *devptr, char* buf, uint32 len) {
pid32 pipe_id;
pipe_id=did32_to_pipid32(devptr->dvnum);

// LAB2: TODO

struct pipe_t *pip_ptr;
intmask mask;




int32 nread;
int32 first_ch;
char ch;


mask=disable();


if( pipe_id < 0 || pipe_id >MAXPIPES || len<0)
        {

                restore(mask);
                return SYSERR;


        }



if (len==0)
{

restore(mask);
return 0;
}



pip_ptr=&pipe_tables[pipe_id];

if(pip_ptr->reader!=currpid)
{


    restore(mask);
    return SYSERR;

 }



if(pip_ptr->state==PIPE_FREE)
{

restore(mask);
return SYSERR;
}




if(pip_ptr->state==PIPE_WRITE_DISCONNECTED)      //disconnected by reader
{

pipdisconnect(pipid32_to_did32(pipe_id));
restore(mask);
return OK;
}



first_ch=pipgetc(devptr);
if(first_ch == SH_EOF )
{

kill(currpid);
restore(mask);
return SH_EOF;

}


//imitated textbook chapter 15 for reading charcters from a device
ch= (char ) first_ch;
*buf++=ch;
nread=1;


while( (nread < len) && (ch != SH_NEWLINE) &&  (ch!= SH_RETURN) && (ch!= SH_EOF) ) {


ch= (char) pipgetc(devptr);
*buf++=ch;
nread++;
}





restore(mask);
return nread;



                     

}
