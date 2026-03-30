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

int TableMake(aSubRecord *precord) {
//    printf("Hello from TableMake....\n");
    int i;
    int capture = *(int *)precord->e;
    int count = *(int *)precord->f;
    float *AdcAvg = (float *)precord->g;
    
    int BLsum, adcBL;
    int32_t AdcCorr[16000];

 //   printf("Capture = %d   Count = %d\n",capture,count);
    if(capture==1 && count<21){
        count = count + 1;
        // First Calculate the Baseline value for the ADC Wfm
        // from Index=0 to Index = (TP1_adc_delay - 200)
        BLsum = 0;
        for(i=0;i<(tp1Delay-200);i++){
            BLsum = BLsum + Adc[i];
        }
        adcBL = (int)(BLsum/(tp1Delay-200));
 //       printf("%d  %d  %d\n",tp1Delay,tp2Delay,tp3Delay);
 //       printf("ADC Baseline from index 0 to %d = %d\n",(tp1Delay-200),adcBL);
        // Remove the baseline value from all ADC Wfm entries...
        for(i=0;i<16000;i++){
            Adc[i] = Adc[i] - adcBL;
        }
        // For TP1 remove the positive charge
        for(i=0;i<1400;i++){
            if(Adc[i+tp1Delay-100]>0) Adc[i+tp1Delay-100] = 0;
            if(Adc[i+tp2Delay-100]<0) Adc[i+tp2Delay-100] = 0;
            if(Adc[i+tp3Delay-100]<0) Adc[i+tp3Delay-100] = 0;
        }
        // Add back the baseline to complete the correction
        // Calculate the new average values
        for(i=0;i<16000;i++){
            Adc[i] = Adc[i] + adcBL;
            AdcAvg[i] = (Adc[i] + (count-1)*AdcAvg[i])/count;
            if(count==20){
                AdcCorr[i] = (int)(AdcAvg[i]);
            }
        }
        *(int *)precord->vala = adcBL;
        if(count==20){
            memcpy((int32_t *)precord->valg,AdcCorr,16000*sizeof(int32_t));
            capture = 0;
            printf("Why?\n");
            *(int *)precord->vali = capture;
        }else{
            capture = 1;
            *(int *)precord->vali = capture;
        }
    }else{
        count = 0;
        for(i=0;i<16000;i++){
            AdcAvg[i] = 0;
        }       
    }
    *(int *)precord->valc = count;
    memcpy((float *)precord->valb,AdcAvg,16000*sizeof(float));
    return(0);
}
// Note the function must be registered at the end!
epicsRegisterFunction(TailSub2);

