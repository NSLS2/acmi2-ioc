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

#include <aSubRecord.h>
#include <registryFunction.h>
#include <epicsExport.h>
#include <epicsString.h>

static int scanCOWDirectory(const char *COWpath, int *numfiles, char files[35][40], char dates[35][40], int fileID[35]) {
    DIR *dir;
    struct dirent *entry;
    time_t timestamp;
    struct tm tm_info;

    *numfiles = 0;
    dir = opendir(COWpath);
    if (dir == NULL) {
        perror("opendir");
        return 1;
    }

    while ((entry = readdir(dir)) != NULL) {
        long long tsll;
        if ((strcmp(entry->d_name, ".") == 0) || (strcmp(entry->d_name, "..") == 0)) {
            continue;
        }
        if (sscanf(entry->d_name, "COW%lld.txt", &tsll) != 1) {
            continue;
        }
        if (*numfiles >= 35) {
            break;
        }
        strncpy(files[*numfiles], entry->d_name, sizeof(files[*numfiles]));
        files[*numfiles][sizeof(files[*numfiles]) - 1] = '\0';
        timestamp = (time_t)tsll;
        localtime_r(&timestamp, &tm_info);
        strftime(dates[*numfiles], 40, "%a %d %b %Y %H:%M:%S %Z", &tm_info);
        fileID[*numfiles] = (int)tsll;
        (*numfiles)++;
    }
    closedir(dir);

    // Sort from newest timestamp to oldest.
    for (int i = 0; i < *numfiles - 1; i++) {
        for (int j = i + 1; j < *numfiles; j++) {
            if (fileID[i] < fileID[j]) {
                int tmpID;
                char tmpFile[40], tmpDate[40];
                tmpID = fileID[i];
                fileID[i] = fileID[j];
                fileID[j] = tmpID;
                snprintf(tmpFile, sizeof(tmpFile), "%s", files[i]);
                snprintf(files[i], sizeof(files[i]), "%s", files[j]);
                snprintf(files[j], sizeof(files[j]), "%s", tmpFile);
                snprintf(tmpDate, sizeof(tmpDate), "%s", dates[i]);
                snprintf(dates[i], sizeof(dates[i]), "%s", dates[j]);
                snprintf(dates[j], sizeof(dates[j]), "%s", tmpDate);
            }
        }
    }

    if (*numfiles > 32) {
        *numfiles = 32;
    }
    return 0;
}

int COWsub(aSubRecord *precord) {
//    printf("Hello from COWSub....\n");
    int i,j,IMAX=0;
    float CFIT[128],CSUM=0,SCORE=0,PMAX=0;
    double *CWFM = (double *)precord->a;
    char *COWpath = (char *)precord->i;  //COWpath is the path to the directory of COW Wfm files
    
//  Added for COW waveform directory management (12/30/2025):
    int fileID[35], numfiles = 0;
    char tmstr[40];
    char files[35][40]={{0}};
    char dates[35][40]={{0}};
    
    int INTG = *(int *)precord->b;
    int PEAK = *(int *)precord->c;
    int FWHM = *(int *)precord->d;
    int INDX = *(int *)precord->e;
    int BASE = *(int *)precord->f;
    int QCAL = *(int *)precord->g;
    int tmsec = *(int *)precord->h;
    char fname[80];
    
  //  printf("COW: tmsec from ARTIX =%d   INTG=%d\n",tmsec,INTG);
    
    

    float Q = 0.0f;
    if (QCAL != 0) {
        Q = (float)INTG/QCAL;
    }
    *(float *)precord->vala = Q;
    
    time_t trigTime;
    struct tm ts;

    if(INTG>0){
        trigTime = (time_t)tmsec;
        ts = *localtime(&trigTime);
        strftime(tmstr,sizeof(tmstr), "%a %d %b %Y %H:%M:%S %Z", &ts);
        sprintf(fname,"%sCOW%d.txt",COWpath,tmsec);
  //      printf("COW: tmsec from EVR = %d   INTG=%d\n",tmsec,INTG);
  //      printf("COW:fname = %s\n",fname);
    
        *(float *)precord->valc = Q;
        *(int *)precord->vald = INTG;
        *(int *)precord->vale = PEAK;
        *(int *)precord->valf = FWHM;
        *(int *)precord->valg = INDX;
        *(int *)precord->valh = BASE;
        snprintf((char *)precord->valj, 80, "%s", tmstr);

        for(i=0;i<128;i++){
            if(CWFM[i]>PMAX){
                PMAX = CWFM[i];
                IMAX = i;
            }
        }
        for(i=0;i<128;i++){
            CFIT[i] = PEAK*exp(-2.7726*(i-IMAX)*(i-IMAX)/(FWHM*FWHM));
        }
        for(i=0;i<128;i++){
            CSUM = CSUM + (CWFM[i]-CFIT[i])*(CWFM[i]-CFIT[i]);
        }
        SCORE = sqrt(CSUM)/PEAK;
   //     printf("CSUM = %8.2f  Score = %8.4f\n",CSUM,SCORE);
   //     printf("COW Integral = %d\n",INTG);
        FILE *fout = fopen(fname,"w");
        if (fout == NULL) {
            perror("fopen");
            return 1;
        }
//        printf("COW: Opening new file: %s\n",fname);
    
        for(i=0;i<16;i=i+1){
            for(j=0;j<7;j=j+1){
                fprintf(fout,"%d,",(int)CWFM[i*8+j]);
                //printf("Writing: i=%d j=%d CWFM[%d]=%d\n",i,j,(i*8+j),(int)CWFM[i*8+j]);
            }
            fprintf(fout,"%d\n",(int)CWFM[i*8+7]);
    //        printf("Writing: i=%d j=7 CWFM[%d]=%d\n",i,(i*8+7),(int)CWFM[i*8+7]);
        }
        fprintf(fout,"%d,%d,%d,%d,%d,%d,%d\n",(int)(1000*Q),INTG,PEAK,FWHM,INDX,BASE,tmsec);
        //printf("%d,%d,%d,%d,%d,%d,%d\n",(int)(1000*Q),INTG,PEAK,FWHM,INDX,BASE,tmsec);
        fclose(fout);  
        
//        printf("COW: File Written...\n");

        memcpy((double *)precord->valb,CWFM,128*sizeof(double));
    }

    if (scanCOWDirectory(COWpath, &numfiles, files, dates, fileID) != 0) {
        return 1;
    }

    *(int *)precord->valk = numfiles;
    precord->novl = numfiles;
    precord->novm = numfiles;
    precord->novn = numfiles;
    char (*fnam)[40] = precord->vall;
    char (*fdat)[40] = precord->valm;

    for(i=0;i<32;i++){
        snprintf(fnam[i], 40, "%s", files[i]);
        snprintf(fdat[i], 40, "%s", dates[i]);
    }
    if (numfiles < 32) {
        memset(fnam[numfiles], 0, 40);
        memset(fdat[numfiles], 0, 40);
    }
    precord->nevn = numfiles;
    memcpy((int *)precord->valn,fileID,32*sizeof(int));

//    printf("End of COWsub\n");
    return(0);
}
// Note the function must be registered at the end!
epicsRegisterFunction(COWsub);

