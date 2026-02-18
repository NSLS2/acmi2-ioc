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

int ADCsub(aSubRecord *precord) {
    printf("Hello from ADCSub....\n");
    int i,indx1,indx2,indx3,indxb,TP1[92],TP2[92],TP3[92],BM[92];
    int BLsum[4]={0},BL[4],pos1,pos2,pos3,neg1,neg2,neg3;
    int INTG[4]={0},PEAK[4]={0},FWHM[4],INDX[4]={0},faults;
    int FWLO[4]={0},FWHI[4]={0};
    int qcal,tmsec;

    time_t trigTime;
    struct tm ts;
    char tmstr[40];

    int *Adc = (int *)precord->a;
    float *STAT1 = (float *)precord->n;
    float *STAT2 = (float *)precord->o;
    float *STAT3 = (float *)precord->p;
    float *STATB = (float *)precord->r;

    indx1 = *(int *)precord->b;
    indx2 = *(int *)precord->c;
    indx3 = *(int *)precord->d;
    pos1 = *(int *)precord->e;
    pos2 = *(int *)precord->f;
    pos3 = *(int *)precord->g;
    neg1 = *(int *)precord->h;
    neg2 = *(int *)precord->i;
    neg3 = *(int *)precord->j;
    faults = *(int *)precord->k;
    qcal = *(int *)precord->l;
    tmsec = *(int *)precord->m;
    indxb = *(int *)precord->q;

    trigTime = (time_t)tmsec;
    ts = *localtime(&trigTime);
    strftime(tmstr,sizeof(tmstr), "%a %d %b %Y %H:%M:%S %Z", &ts);

    for(i=0;i<92;i++){
        TP1[i] = ((pos1>0)-(neg1>0))*Adc[indx1+i];
        TP2[i] = ((pos2>0)-(neg2>0))*Adc[indx2+i];
        TP3[i] = ((pos3>0)-(neg3>0))*Adc[indx3+i];
        BM[i] = -1*Adc[indxb+i];
        if(i<32){
           BLsum[0] = BLsum[0] + TP1[i];
           BLsum[1] = BLsum[1] + TP2[i];
           BLsum[2] = BLsum[2] + TP3[i];
           BLsum[3] = BLsum[3] + BM[i];
        }
    }
    BL[0] = round((float)BLsum[0]/32.0);
    BL[1] = round((float)BLsum[1]/32.0);
    BL[2] = round((float)BLsum[2]/32.0);
    BL[3] = round((float)BLsum[3]/32.0);
    printf("BLsum[3]=%d\n",BLsum[3]);

    memcpy((int *)precord->vala,TP1,92*sizeof(int));
    memcpy((int *)precord->valb,TP2,92*sizeof(int));
    memcpy((int *)precord->valc,TP3,92*sizeof(int));
    memcpy((int *)precord->valq,BM,92*sizeof(int));
    printf("Hello\n");
    for(i=32;i<92;i++){
        INTG[0] = INTG[0] + TP1[i] - BL[0];
        if(TP1[i]>PEAK[0]){
            PEAK[0] = TP1[i];
            INDX[0] = i;
        }
        INTG[1] = INTG[1] + TP2[i] - BL[1];
        if(TP2[i]>PEAK[1]){
            PEAK[1] = TP2[i];
            INDX[1] = i;
        }
        INTG[2] = INTG[2] + TP3[i] - BL[2];
        if(TP3[i]>PEAK[2]){
            PEAK[2] = TP3[i];
            INDX[2] = i;
        }
        INTG[3] = INTG[3] + BM[i] - BL[3];
        if(BM[i]>PEAK[3]){
            PEAK[3] = BM[i];
            INDX[3] = i;
        }
    }
    PEAK[0] = PEAK[0] - BL[0];
    INDX[0] = INDX[0] + indx1;
    PEAK[1] = PEAK[1] - BL[1];
    INDX[1] = INDX[1] + indx2;
    PEAK[2] = PEAK[2] - BL[2];
    INDX[2] = INDX[2] + indx3;
    PEAK[3] = PEAK[3] - BL[3];
    INDX[3] = INDX[3] + indxb;

    for(i=32;i<92;i++){
        if(FWLO[0]==0 && TP1[i]>(PEAK[0]/2)) FWLO[0] = i;
        if(FWLO[0]>0 && FWHI[0]==0 && TP1[i]<(PEAK[0]/2)) FWHI[0] = i;
        if(FWLO[1]==0 && TP2[i]>(PEAK[1]/2)) FWLO[1] = i;
        if(FWLO[1]>0 && FWHI[1]==0 && TP2[i]<(PEAK[1]/2)) FWHI[1] = i;
        if(FWLO[2]==0 && TP3[i]>(PEAK[2]/2)) FWLO[2] = i;
        if(FWLO[2]>0 && FWHI[2]==0 && TP3[i]<(PEAK[2]/2)) FWHI[2] = i;
        if(FWLO[3]==0 && BM[i]>(PEAK[3]/2)) FWLO[3] = i;
        if(FWLO[3]>0 && FWHI[3]==0 && BM[i]<(PEAK[3]/2)) FWHI[3] = i;
    }

    FWHM[0] = FWHI[0]-FWLO[0];
    FWHM[1] = FWHI[1]-FWLO[1];
    FWHM[2] = FWHI[2]-FWLO[2];
    FWHM[3] = FWHI[3]-FWLO[3];

    STAT1[0] = (float)INTG[0];
    STAT1[1] = (float)PEAK[0];
    STAT1[2] = (float)FWHM[0];
    STAT1[3] = (float)INDX[0];
    STAT1[4] = (float)BL[0];
    STAT1[5] = STAT1[0]/qcal;

    STAT2[0] = (float)INTG[1];
    STAT2[1] = (float)PEAK[1];
    STAT2[2] = (float)FWHM[1];
    STAT2[3] = (float)INDX[1];
    STAT2[4] = (float)BL[1];
    STAT2[5] = STAT2[0]/qcal;

    STAT3[0] = (float)INTG[2];
    STAT3[1] = (float)PEAK[2];
    STAT3[2] = (float)FWHM[2];
    STAT3[3] = (float)INDX[2];
    STAT3[4] = (float)BL[2];
    STAT3[5] = STAT3[0]/qcal;

    STATB[0] = (float)INTG[3];
    STATB[1] = (float)PEAK[3];
    STATB[2] = (float)FWHM[3];
    STATB[3] = (float)INDX[3];
    STATB[4] = (float)BL[3];
    STATB[5] = STATB[0]/qcal;

    printf("%s  %d\n",tmstr,strlen(tmstr));
    precord->nevm = strlen(tmstr);
    memcpy((float *)precord->valg,STAT1,12*sizeof(float));
    memcpy((float *)precord->valh,STAT2,12*sizeof(float));
    memcpy((float *)precord->vali,STAT3,12*sizeof(float));
    memcpy((float *)precord->valr,STATB,12*sizeof(float));
    memcpy((char *)precord->valm,tmstr,strlen(tmstr)*sizeof(char));
    printf("Faults_tp = 0x%x\n",faults);
// If a TP1 FAULT occurred on this trigger then update the TP1 Fault PVs...
    if((faults&0x0000FF)>0){
        memcpy((int *)precord->vald,TP1,92*sizeof(int));
        memcpy((float *)precord->valj,STAT1,12*sizeof(float));
        memcpy((char *)precord->valn,tmstr,strlen(tmstr)*sizeof(char));
    }
// If a TP2 FAULT occurred on this trigger then update the TP2 Fault PVs...
    if((faults&0x00FF00)>0){
        memcpy((int *)precord->vale,TP2,92*sizeof(int));
        memcpy((float *)precord->valk,STAT2,12*sizeof(float));
        memcpy((char *)precord->valo,tmstr,strlen(tmstr)*sizeof(char));
    }
// If a TP3 FAULT occurred on this trigger then update the TP3 Fault PVs...
    if((faults&0xFF0000)>0){
        //printf("Updating the TP3 Fault Waveform\n");
        memcpy((int *)precord->valf,TP3,92*sizeof(int));
        memcpy((float *)precord->vall,STAT3,12*sizeof(float));
        memcpy((char *)precord->valp,tmstr,strlen(tmstr)*sizeof(char));
    }
    return(0);
}
// Note the function must be registered at the end!
epicsRegisterFunction(ADCsub);

