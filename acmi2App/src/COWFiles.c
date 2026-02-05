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
#include <dirent.h>
#include <errlog.h>

#include <aSubRecord.h>
#include <registryFunction.h>
#include <epicsExport.h>

int COWFiles(aSubRecord *precord){
    printf("Hello from COWFiles....\n");
    
    char fname[80];

    int index = *(int *)precord->a;
    int *FileID = (int *)precord->b;
    sprintf(fname,"COW%d.txt",FileID[index]);
    
    char *out = (char *)precord->vala;
    snprintf(out, 40,"%s",fname);
    
    char line[80],lines[17][80];
    char fullname[80];
    snprintf(fullname,23+strlen(fname),"/home/diag/acmi2/COW/%s",fname);

    FILE *fp = fopen(fullname,"r");
    
    int lnum = 0;
    if(!fp){
        printf("Error opening %s\n",fname);
    }else{
        while (fgets(line, sizeof(line), fp) != NULL) {
            strncpy(lines[lnum], line, sizeof(lines[lnum]));
            lines[lnum][sizeof(lines[lnum])-1] = '\0';
            lnum++;
        }
    }
    int array[128];
    int indx = 0,intg,fwhm,peak,base;
    for(int j=0;j<16;j++){
        char *token = strtok(lines[j], ",");
        while (token != NULL) {
            array[indx] = atoi(token);
            indx++;
            token = strtok(NULL, ",");
        }
    }
    precord->nevb = 128;
    memcpy((int *)precord->valb,array,128*sizeof(int));
    char *token = strtok(lines[16], ",");
    float Q = (float)(atoi(token))/1000.0;
    token = strtok(NULL, ",");
    intg = atoi(token);
    token = strtok(NULL, ",");
    peak = atoi(token);
    token = strtok(NULL, ",");
    fwhm = atoi(token);
    token = strtok(NULL, ",");
    indx = atoi(token);
    token = strtok(NULL, ",");
    base = atoi(token);
    
    *(float *)precord->valc = Q;
    *(int *)precord->vald = intg;
    *(int *)precord->vale = peak;
    *(int *)precord->valf = fwhm;
    *(int *)precord->valg = indx;
    *(int *)precord->valh = base;
    return(0);
}
 // Note the function must be registered at the end!
epicsRegisterFunction(COWFiles);
