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

#include <dbAccess.h>    // for db_post_events
#include <epicsThread.h>
#include <aSubRecord.h>
#include <registryFunction.h>
#include <epicsExport.h>

/* Structure to pass arguments to the thread */
typedef struct {
    DBLINK ADCwfmLink;  // INPB: ADC-Wfm
    DBLINK TP1DelLink;  // INPC: EE:TP1_adc_delay-I
    DBLINK TP2DelLink;  // INPD: EE:TP2_adc_delay-I
    DBLINK TP3DelLink;  // INPE: EE:TP3_adc_delay-I
    DBLINK ADCavgInLink;  // INPF: ADC:Avg-Wfm

    DBLINK countLink;   // OUTA: ADC:Count-I
    DBLINK runLink;     // OUTB: RUN
    DBLINK baseLink;    // OUTC: ADC:Baseline-I
    DBLINK ADCavgOutLink;  // OUTD: ADC:Avg-Wfm
    DBLINK ADCcorrLink;  // OUTE: ADC:Corr-Wfm
} loopArgs;

/* Thread function to perform the 20 iterations */
static void loopThread(void *arg)
{
    printf("Hello\n");
    loopArgs *la = (loopArgs *)arg;
    int count;
    int runVal;
    int ADC[16000], BLsum, TP1delay=0, TP2delay=0, TP3delay=0, adcBL;
    int i,ADCcorr[16000];
    float ADCavg[16000];
    long Nwfm=16000, Navg=16000, Ncor=16000;
    // RUN=1
    runVal = 1;
    dbPutLink(&la->runLink, DBR_LONG, &runVal, 1);
    epicsThreadSleep(0.05);
    for (count = 1; count <= 20; count++) {
        dbGetLink(&la->ADCwfmLink, DBR_LONG, ADC, 0, &Nwfm);
        dbGetLink(&la->ADCavgInLink, DBR_FLOAT, ADCavg, 0, &Navg);
        dbGetLink(&la->TP1DelLink, DBR_LONG, &TP1delay, 0, 0);
        dbGetLink(&la->TP2DelLink, DBR_LONG, &TP2delay, 0, 0);
        dbGetLink(&la->TP3DelLink, DBR_LONG, &TP3delay, 0, 0);
        epicsThreadSleep(0.05);
        // First Calculate the Baseline value for the ADC Wfm
        // from Index=0 to Index = (TP1_adc_delay - 200)
        BLsum = 0;
        for(i=0;i<(TP1delay-200);i++){
            BLsum = BLsum + ADC[i];
        }
        adcBL = (int)(BLsum/(TP1delay-200));
        // Remove the baseline value from all ADC Wfm entries...
        for(i=0;i<16000;i++){
            ADC[i] = ADC[i] - adcBL;
        }
        // For TP1 remove the positive charge
        for(i=0;i<1400;i++){
            if(ADC[i+TP1delay-100]>0) ADC[i+TP1delay-100] = 0;
            if(ADC[i+TP2delay-100]<0) ADC[i+TP2delay-100] = 0;
            if(ADC[i+TP3delay-100]<0) ADC[i+TP3delay-100] = 0;
        }
        
        // Add back the baseline to complete the correction
        // Calculate the new average values
        for(i=0;i<16000;i++){
            ADC[i] = ADC[i] + adcBL;
            ADCavg[i] = (ADC[i] + (count-1)*ADCavg[i])/count;
            if(count==20){
                ADCcorr[i] = (int)(ADCavg[i]);
            }
        }
        if(count==20) dbPutLink(&la->ADCcorrLink, DBR_LONG, ADCcorr, &Ncor);
        dbPutLink(&la->ADCavgOutLink, DBR_FLOAT, ADCavg, &Navg);
        dbPutLink(&la->countLink, DBR_LONG, &count, 1);
        printf("Iteration: %d\n", count);
        dbPutLink(&la->baseLink, DBR_LONG, &adcBL, 1);
        epicsThreadSleep(1.0);  // 1 second delay
    }

    // Reset RUN=0 and COUNT=0
    runVal = 0;
    dbPutLink(&la->runLink, DBR_LONG, &runVal, 1);

    count = 0;
    dbPutLink(&la->countLink, DBR_LONG, &count, 1);

    epicsThreadSleep(0.05);

    free(la);
}

/* aSub function triggered by START */
long myAsubFunc(aSubRecord *prec)
{
    long start = *(long *)prec->a;
    if (!start) return 0;

    // Allocate arguments for the thread
    loopArgs *arg = malloc(sizeof(loopArgs));
    if (!arg) return -1;

    arg->ADCwfmLink = prec->inpb; // INPB: ADC-Wfm
    arg->TP1DelLink = prec->inpc; // INPC: EE:TP1_adc_delay-I
    arg->TP2DelLink = prec->inpd; // INPD: EE:TP2_adc_delay-I
    arg->TP3DelLink = prec->inpe; // INPE: EE:TP3_adc_delay-I
    arg->ADCavgInLink = prec->inpf; // INPB: ADC-Wfm
    
    arg->countLink = prec->outa;  // OUTA: Count-I
    arg->runLink   = prec->outb;  // OUTB: RUN
    arg->baseLink  = prec->outc;  // OUTC: ADC:Baseline-I
    arg->ADCavgOutLink = prec->outd; // OUTD: ADC-Wfm
    arg->ADCcorrLink = prec->oute; // OUTD: ADC:Corr-Wfm
    
    // Spawn a thread for the loop
    epicsThreadCreate("aSubLoopThread",
                      epicsThreadPriorityMedium,
                      epicsThreadGetStackSize(epicsThreadStackMedium),
                      (EPICSTHREADFUNC)loopThread,
                      arg);

    return 0;
}
epicsRegisterFunction(myAsubFunc);
