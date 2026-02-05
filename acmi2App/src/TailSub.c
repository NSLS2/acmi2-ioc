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

int TailSub(aSubRecord *precord) {
    //printf("Hello from TailSub....\n");
    int i;
    int *Adc = (int *)precord->a;
    int delay = *(int *)precord->b;
    int capture = *(int *)precord->c;
    int apply = *(int *)precord->d;
    int startIndx = *(int *)precord->e;
    int *AvgTail = (int *)precord->f;
    int count = *(int *)precord->g;
    
    int TP3Raw[2000],TP3Corrected[2000],Tail[2000];
    
    for(i=0;i<2000;i++){
        TP3Raw[i] = Adc[delay-100+i];
    }
    
    for(i=0;i<startIndx;i++){
        Tail[i] = 0;
    }
    for(i=startIndx;i<2000;i++){
        Tail[i] = TP3Raw[i];
    }
    
    if(capture==1){
        if(count==0){
            memcpy((int *)precord->valc,Tail,2000*sizeof(int));
            count++;
            *(int *)precord->vald = count;
            *(int *)precord->vale = capture;
        }else{
            //printf("count = %d\n",count);
            for(i=0;i<2000;i++){
                AvgTail[i] = (count*AvgTail[i] + Tail[i])/(count+1);
            }
            memcpy((int *)precord->valc,AvgTail,2000*sizeof(int));
            count++;
            *(int *)precord->vald = count;
        }
        if(count==20){
            capture = 0;
            count = 0;
            *(int *)precord->vale = capture;
            *(int *)precord->vald = count;
        }
    }
    
    if(apply==1){
        for(i=0;i<2000;i++){
            TP3Corrected[i] = TP3Raw[i] - AvgTail[i];
        }
        memcpy((int *)precord->valf,TP3Corrected,2000*sizeof(int));
    }
    
    //printf("delay=%d  capture=%d  apply=%d  startIndx=%d\n",delay,capture,apply,startIndx);
    
    memcpy((int *)precord->vala,TP3Raw,2000*sizeof(int));
    memcpy((int *)precord->valb,Tail,2000*sizeof(int));
    
    return(0);
}
// Note the function must be registered at the end!
epicsRegisterFunction(TailSub);
