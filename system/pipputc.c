#include <xinu.h>

devcall pipputc(struct dentry *devptr, char ch) {


intmask mask; /* saved interrupt mask */
 
int32 retval; /* value to return to caller */
did32 descrp=devptr->dvnum;

mask = disable();




if (isbaddev(descrp)) {
restore(mask);
return SYSERR;
}



devptr = (struct dentry *) &devtab[descrp];

retval = (*devptr->dvputc) (devptr, ch);

restore(mask);
return retval;



    // LAB2: TODO
   // return OK;
}

