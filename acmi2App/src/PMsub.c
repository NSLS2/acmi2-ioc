#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stddef.h>
#include <string.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <stdint.h>
#include <unistd.h>
#include <math.h>
#include <time.h>
#include <aSubRecord.h>
#include <registryFunction.h>
#include <epicsExport.h>

int PMsub(aSubRecord *precord) {
    //printf("Hello from PMSub....\n");

    int armed = *(int *)precord->b;
    int process=0;
    double *PM = (double *)precord->a;
    //printf("PM: old=%5.0f, new=%5.0f\n",PM[0],PM[1]);
    if(PM[1]==0 && PM[0]!=0){
        armed = 1;
        //printf("PM Armed.\n");
    }else{
        if(PM[1]!=0 && PM[0]==0){ 
            armed = 0;
            process = 1;
            //printf("Generate a PM Report\n");
        }
    }
    
    *(int *)precord->vala = armed;
    *(int *)precord->valb = process;
    return(0);
}
// Note the function must be registered at the end!
epicsRegisterFunction(PMsub);
    
