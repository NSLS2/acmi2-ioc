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

#define WMAX 1000

int BMsub(aSubRecord *precord) {
//    printf("Hello from BMSub....\n");
    int i;

    float *STAT = (float *)precord->a;
    float *QWFM = (float *)precord->b;
    float *AWFM = (float *)precord->h;

    int INTG = *(int *)precord->c;
    int RESET = *(int *)precord->d;
    int WLEN = *(int *)precord->e;
    int QCAL = *(int *)precord->f;
    int ACCU = *(int *)precord->g;

    if(RESET>0){
        WLEN = 0;
        RESET = 0;
        *(int *)precord->vald = RESET;
    }

    if(WLEN>0 || INTG>0){
        float Q = (float)INTG/QCAL;
        float QACC = (float)ACCU/QCAL;
        if(WLEN<WMAX){
            QWFM[WLEN] = Q;
            AWFM[WLEN] = QACC;
            WLEN = WLEN + 1;
        }else{
            memmove(QWFM,QWFM+1,(WMAX-1)*sizeof(float));
            QWFM[WMAX-1] = Q;
            memmove(AWFM,AWFM+1,(WMAX-1)*sizeof(float));
            AWFM[WMAX-1] = QACC;
            WLEN = WMAX;
        }
        
//        printf("Q=%5.3fnC  QACC=%8.3fnC\n",Q,QACC);
        *(float *)precord->vala = Q;
        *(float *)precord->valf = QACC;
        *(int *)precord->vale = WLEN;
    }

    float QSUM=0;
    for(i=0;i<WLEN;i=i+1){
        QSUM = QSUM + QWFM[i];
    }
    float QAVG = QSUM/WLEN;

    STAT[6] = QAVG;
    QSUM = 0;
    for(i=0;i<WLEN;i=i+1){
        QSUM = QSUM + (QWFM[i]-QAVG)*(QWFM[i]-QAVG);
    }
    float QSTD = sqrt(QSUM/WLEN);
    STAT[9] = QSTD;

    precord->nevb = WLEN;
    precord->nevg = WLEN;
    memcpy((float *)precord->valb,QWFM,WLEN*sizeof(float));
    memcpy((float *)precord->valg,AWFM,WLEN*sizeof(float));
    memcpy((float *)precord->valc,STAT,12*sizeof(float));

    return(0);
}
// Note the function must be registered at the end!
epicsRegisterFunction(BMsub);

