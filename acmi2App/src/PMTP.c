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

int PMTP(aSubRecord *precord) {
    //printf("Hello from PMTP....***********\n");
    
    int process = *(int *)precord->p;
    //printf("Process = %d\n",process);
    

    if(process==1){
        int A[19]={0};
        int *B = (int *)precord->o;

        float Q = *(float *)precord->a;
        A[0]= (int)(Q*10000);
        A[1]= *(int *)precord->b;
        A[2]= *(int *)precord->c;
        A[3]= *(int *)precord->d;
        A[4]= *(int *)precord->e;
        A[5]= *(int *)precord->f;
        A[6]= *(int *)precord->g;
        A[7]= *(int *)precord->h;
        A[8]= *(int *)precord->i;
        A[9]= *(int *)precord->j;
        A[10]= *(int *)precord->k;
        A[11]= *(int *)precord->l;
        A[12]= *(int *)precord->m;
        A[13]= *(int *)precord->n;
        A[14]= *(int *)precord->q;
        A[15]= *(int *)precord->r;
        A[16]= *(int *)precord->s;
        A[17]= *(int *)precord->t;
        A[18]= *(int *)precord->u;
        
        //for(int i=0;i<19;i++){
            //printf("A[%d] = %d\n",i,A[i]);
        //}
     
        *(float *)precord->vala = Q;
        *(int *)precord->valb = A[1];
        *(int *)precord->valc = A[2];
        *(int *)precord->vald = A[3];
        *(int *)precord->vale = A[4];
        *(int *)precord->valf = A[5];
        *(int *)precord->valg = A[6];
        *(int *)precord->valh = A[7];
        *(int *)precord->vali = A[8];
        *(int *)precord->valj = A[9];
        *(int *)precord->valk = A[10];
        *(int *)precord->vall = A[11];
        *(int *)precord->valm = A[12];
        *(int *)precord->valn = A[13];
     
        memcpy((int *)precord->valo,A,19*sizeof(int));
        memcpy((int *)precord->valp,B,92*sizeof(int));
    } 
    return(0);
}
// Note the function must be registered at the end!
epicsRegisterFunction(PMTP);
