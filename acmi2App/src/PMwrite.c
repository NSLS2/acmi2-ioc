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

int PMwrite(aSubRecord *precord) {
    //printf("Hello from PMwrite....\n");
    
    int process = *(int *)precord->l;
    
    if(process==1){
        int i,j;
        char fname[80];
        int *TP1data = (int *)precord->a;
        int *TP2data = (int *)precord->b;
        int *TP3data = (int *)precord->c;
        int *BMdata = (int *)precord->d;
        int *COWdata = (int *)precord->e;
        int *TP1wfm = (int *)precord->f;
        int *TP2wfm = (int *)precord->g;
        int *TP3wfm = (int *)precord->h;
        int *BMwfm = (int *)precord->i;
        int *COWwfm = (int *)precord->j;
        float *ACCwfm = (float *)precord->k;
        
        sprintf(fname,"/home/diag/acmi2/PM/PM%d.txt",COWdata[13]);
    
        FILE *fout = fopen(fname,"w");
    
        for(i=0;i<18;i=i+1){
            fprintf(fout,"%d,%d,%d,%d,%d\n",TP1data[i],TP2data[i],TP3data[i],BMdata[i],COWdata[i]);
        }
        fprintf(fout,"%d,%d,%d,%d,0\n\n",TP1data[18],TP2data[18],TP3data[18],BMdata[18]);
        for(i=0;i<92;i=i+1){
            fprintf(fout,"%d,%d,%d,%d\n",TP1wfm[i],TP2wfm[i],TP3wfm[i],BMwfm[i]);
        }
        fprintf(fout,"\n\n");
        for(i=0;i<16;i=i+1){
            for(j=0;j<7;j=j+1){
                fprintf(fout,"%d,",COWwfm[i*8+j]);
            }
            fprintf(fout,"%d\n",COWwfm[i*8+7]);
        }
        fprintf(fout,"\n\n");
        for(i=0;i<100;i=i+1){
            for(j=0;j<9;j=j+1){
                fprintf(fout,"%5.3f,",ACCwfm[i*10+j]);
            }
            fprintf(fout,"%5.3f\n",ACCwfm[i*10+9]);
        }
        fclose(fout);   
    }
    return(0);
}
// Note the function must be registered at the end!
epicsRegisterFunction(PMwrite);

