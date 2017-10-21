#include <xinu.h>

devcall pipgetc(struct dentry *devptr) {
  
intmask mask; /* saved interrupt mask */

int32 retval; /* value to return to caller */
mask = disable();


did32 descrp=devptr->dvnum;

if (isbaddev(descrp)) {
restore(mask);
return SYSERR;
}




devptr = (struct dentry *) &devtab[descrp];
retval = (*devptr->dvgetc) (devptr);
restore(mask);
return retval;






  // LAB2: TODO
//    return OK;
}

