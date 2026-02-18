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

int TPsub(aSubRecord *precord) {
    printf("Hello from TPSub....\n");
    int i;

    float *STAT = (float *)precord->d;
    float *QWFM = (float *)precord->e;
    int *PWFM = (int *)precord->f;
    int *BWFM = (int *)precord->g;

    int INTG = *(int *)precord->a;
    int PEAK = *(int *)precord->b;
    int BASE = *(int *)precord->c;
    int RESET = *(int *)precord->h;
    int WLEN = *(int *)precord->i;
    int QCAL = *(int *)precord->j;

    if(RESET>0){
        WLEN = 0;
        RESET = 0;
        *(int *)precord->valf = RESET;
    }

    if(WLEN>0 || PEAK>0){
        float Q = (float)INTG/QCAL;
        if(WLEN<WMAX){
            QWFM[WLEN] = Q;
            PWFM[WLEN] = PEAK;
            BWFM[WLEN] = BASE;
            WLEN = WLEN + 1;
        }else{
            memmove(QWFM,QWFM+1,(WMAX-1)*sizeof(float));
            memmove(PWFM,PWFM+1,(WMAX-1)*sizeof(int));
            memmove(BWFM,BWFM+1,(WMAX-1)*sizeof(int));
            QWFM[WMAX-1] = Q;
            PWFM[WMAX-1] = PEAK;
            BWFM[WMAX-1] = BASE;
            WLEN = WMAX;
        }

        *(float *)precord->vala = Q;
        *(int *)precord->valg = WLEN;
    }

    int PSUM=0,BSUM=0;
    float QSUM=0;
    for(i=0;i<WLEN;i=i+1){
        PSUM = PSUM + PWFM[i];
        BSUM = BSUM + BWFM[i];
        QSUM = QSUM + QWFM[i];
    }
    float QAVG = QSUM/WLEN;
    float PAVG = (float)PSUM/WLEN;
    float BAVG = (float)BSUM/WLEN;
    STAT[6] = QAVG;
    STAT[7] = PAVG;
    STAT[8] = BAVG;
    PSUM = 0;
    BSUM = 0;
    QSUM = 0;
    for(i=0;i<WLEN;i=i+1){
        PSUM = PSUM + (PWFM[i]-PAVG)*(PWFM[i]-PAVG);
        BSUM = BSUM + (BWFM[i]-BAVG)*(BWFM[i]-PAVG);
        QSUM = QSUM + (QWFM[i]-QAVG)*(QWFM[i]-QAVG);
    }
    float QSTD = sqrt(QSUM/WLEN);
    float PSTD = sqrt((float)PSUM/WLEN);
    float BSTD = sqrt((float)BSUM/WLEN);
    STAT[9] = QSTD;
    STAT[10] = PSTD;
    STAT[11] = BSTD;

    precord->nevb = WLEN;
    precord->nevc = WLEN;
    precord->nevd = WLEN;
    memcpy((float *)precord->valb,QWFM,WLEN*sizeof(float));
    memcpy((int *)precord->valc,PWFM,WLEN*sizeof(int));
    memcpy((int *)precord->vald,BWFM,WLEN*sizeof(int));
    memcpy((float *)precord->vale,STAT,12*sizeof(float));
    return(0);
}
// Note the function must be registered at the end!
epicsRegisterFunction(TPsub);

