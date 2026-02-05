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

int PMCOW(aSubRecord *precord) {
    //printf("Hello from PMCOW....*****\n");
    
    int process = *(int *)precord->t;  

    if(process==1){
        int A[19]={0};
        int *B = (int *)precord->h;
        
        time_t trigTime;
        struct tm ts;
        char tmstr[40];
        
        float Q = *(float *)precord->a;
        A[0]= (int)(Q*10000);
        A[1]= *(int *)precord->b;
        A[2]= *(int *)precord->c;
        A[3]= *(int *)precord->d;
        A[4]= *(int *)precord->e;
        A[5]= *(int *)precord->f;
        A[6]= *(int *)precord->g;
        A[7]= *(int *)precord->i;
        A[8]= *(int *)precord->j;
        A[9]= *(int *)precord->k;
        A[10]= *(int *)precord->l;
        A[11]= *(int *)precord->m;
        A[12]= *(int *)precord->n;
        A[13]= *(int *)precord->o;
        A[14]= *(int *)precord->p;
        A[15]= *(int *)precord->q;
        A[16]= *(int *)precord->r;
        A[17]= *(int *)precord->s;

        trigTime = (time_t)A[13];
        ts = *localtime(&trigTime);
        strftime(tmstr,sizeof(tmstr), "%a %d %b %Y %H:%M:%S %Z", &ts);
     
        *(float *)precord->vala = Q;
        *(int *)precord->valb = A[1];
        *(int *)precord->valc = A[2];
        *(int *)precord->vald = A[3];
        *(int *)precord->vale = A[4];
        *(int *)precord->valf = A[5];
        *(int *)precord->valg = A[6];
        *(int *)precord->vali = A[7];
        *(int *)precord->valj = A[8];
        *(int *)precord->valk = A[9];
        *(int *)precord->vall = A[10];
        *(int *)precord->valm = A[11];
        *(int *)precord->valn = A[12];
        *(int *)precord->valo = A[13];
        *(int *)precord->valp = A[14];
        *(int *)precord->valq = A[15];
        *(int *)precord->valr = A[16];
        *(int *)precord->vals = A[17];        
     
        memcpy((int *)precord->valt,A,18*sizeof(int));
        memcpy((int *)precord->valh,B,128*sizeof(int));
        memcpy((char *)precord->valu,tmstr,strlen(tmstr)*sizeof(char));
    } 
    return(0);
}
// Note the function must be registered at the end!
epicsRegisterFunction(PMCOW);
