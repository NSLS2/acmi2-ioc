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

int COWsub(aSubRecord *precord) {
    printf("Hello from COWSub....\n");
    int i,j,IMAX=0;
    float CFIT[128],CSUM=0,SCORE=0,PMAX=0;
    double *CWFM = (double *)precord->a;
    
//  Added for COW waveform directory management (12/30/2025):
    int Ival,fileID[35];
    DIR *dir;
    struct dirent *entry;
    char *token,*end,tmstr[40];
    char files[35][40]={{0}};
    char dates[35][40]={{0}};
    time_t timestamp;
    struct tm tm_info;
    
    int INTG = *(int *)precord->b;
    int PEAK = *(int *)precord->c;
    int FWHM = *(int *)precord->d;
    int INDX = *(int *)precord->e;
    int BASE = *(int *)precord->f;
    int QCAL = *(int *)precord->g;
    int tmsec = *(int *)precord->h;
    char fname[40], fn[40];
    
//    printf("tmsec from ARTIX =%d   INTG=%d\n",tmsec,INTG);
    
    

    float Q = (float)INTG/QCAL;
    *(float *)precord->vala = Q;
    
    time_t trigTime;
    struct tm ts;

    if(INTG>0){
        trigTime = (time_t)tmsec;
        ts = *localtime(&trigTime);
        strftime(tmstr,sizeof(tmstr), "%a %d %b %Y %H:%M:%S %Z", &ts);
        sprintf(fname,"/home/diag/acmi2/COW/COW%d.txt",tmsec);
        printf("tmsec from EVR = %d   INTG=%d\n",tmsec,INTG);
        printf("%s\n",tmstr);
    
        *(float *)precord->valc = Q;
        *(int *)precord->vald = INTG;
        *(int *)precord->vale = PEAK;
        *(int *)precord->valf = FWHM;
        *(int *)precord->valg = INDX;
        *(int *)precord->valh = BASE;
        snprintf((char *)precord->valj, 40, "%s", tmstr);

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
        printf("CSUM = %8.2f  Score = %8.4f\n",CSUM,SCORE);
        printf("COW Integral = %d\n",INTG);
// Write waveform to file:
        // Check COW waveform directory for exisiting files:
        dir = opendir("/home/diag/acmi2/COW");
        if (dir == NULL) {
            perror("opendir");
            return 1;
        }
        // read all file names and identifiers into arrays:
        //Add the new file to the first element of the arrays:
        int numfiles = 0;
        sprintf(fn,"COW%d.txt",tmsec);
        printf("fn=%s\n",fn);
        strncpy(files[numfiles], fn, sizeof(files[numfiles]));
        files[numfiles][sizeof(files[numfiles])-1] = '\0';
        printf("tmstr = %s\n",tmstr);
        strncpy(dates[numfiles], tmstr, sizeof(dates[numfiles]));
        dates[numfiles][sizeof(dates[numfiles])-1] = '\0';
        fileID[numfiles] = (int)tmsec;
        numfiles++;
        while ((entry = readdir(dir)) != NULL) {
            if (strcmp(entry->d_name, ".") == 0 ||
                strcmp(entry->d_name, "..") == 0) {
                continue;
            }
            char nameCopy[40];
            strncpy(files[numfiles], entry->d_name, sizeof(files[numfiles]));
            files[numfiles][sizeof(files[numfiles])-1] = '\0';
            strncpy(nameCopy, entry->d_name, sizeof(nameCopy)-1);
            nameCopy[sizeof(nameCopy)-1] = '\0';
            token = strtok(nameCopy,"W");
            token = strtok(NULL,".");
            timestamp = (time_t)strtoll(token, NULL, 10);
            localtime_r(&timestamp,&tm_info);
            strftime(tmstr,sizeof(tmstr), "%a %d %b %Y %H:%M:%S %Z", &tm_info);
            strncpy(dates[numfiles], tmstr, sizeof(dates[numfiles]));
            dates[numfiles][sizeof(dates[numfiles])-1] = '\0';
            Ival = strtol(token,&end,10);
            fileID[numfiles] = Ival;
            numfiles++;
        }
        // Sort the arrays from newest timestamp to oldest:
        char tmpFile[40],tmpDate[40];
        for (int i = 0; i < numfiles - 1; i++) {
            for (int j = i + 1; j < numfiles; j++) {
                if (fileID[i] < fileID[j]) {
                    int tmp = fileID[i];
                    fileID[i] = fileID[j];
                    fileID[j] = tmp;
                    snprintf(tmpFile, sizeof(tmpFile), "%s", files[i]);
                    snprintf(files[i], sizeof(files[i]), "%s", files[j]);
                    snprintf(files[j], sizeof(files[j]), "%s", tmpFile);

                    snprintf(tmpDate, sizeof(tmpDate), "%s", dates[i]);
                    snprintf(dates[i], sizeof(dates[i]), "%s", dates[j]);
                    snprintf(dates[j], sizeof(dates[j]), "%s", tmpDate);
                }
            }
        }
//        for(int i=0;i<numfiles;i=i+1){
//            printf("fileID[%d] = %d    files[%d] = %s    dates[%d] = %s\n",i,fileID[i],i,files[i],i,dates[i]);
//        }
        for(int i=numfiles;i<32;i=i+1){
            fileID[i] = 0;
            files[i][0] = '\0';
            dates[i][0] = '\0';
        }
        // if numfiles==20 then delete the oldest COW waveform file:
        char fdel[40];
        if(numfiles==33){
            strcpy(fdel,"/home/diag/acmi2/COW/");
            if(remove(strcat(fdel,files[32]))!=0){
                printf("ERROR deleting %s\n",fdel);
            }else{
                printf("Deleted file: %s\n",fdel);
                files[32][0] = '\0';
                dates[32][0] = '\0';
                numfiles = numfiles - 1;
            }
        }

        FILE *fout = fopen(fname,"w");
        printf("Opening new file: %s\n",fname);
    
        for(i=0;i<16;i=i+1){
            for(j=0;j<7;j=j+1){
                fprintf(fout,"%d,",(int)CWFM[i*8+j]);
                printf("Writing: i=%d j=%d CWFM[%d]=%d\n",i,j,(i*8+j),(int)CWFM[i*8+j]);
            }
            fprintf(fout,"%d\n",(int)CWFM[i*8+7]);
            printf("Writing: i=%d j=7 CWFM[%d]=%d\n",i,(i*8+7),(int)CWFM[i*8+7]);
        }
        fprintf(fout,"%d,%d,%d,%d,%d,%d,%d\n",(int)(1000*Q),INTG,PEAK,FWHM,INDX,BASE,tmsec);
        printf("%d,%d,%d,%d,%d,%d,%d\n",(int)(1000*Q),INTG,PEAK,FWHM,INDX,BASE,tmsec);
        fclose(fout);  
        
        printf("File Written...\n");

        memcpy((double *)precord->valb,CWFM,128*sizeof(double));
        *(int *)precord->valk = numfiles;
        precord->novl = numfiles;
        precord->novm = numfiles;
        char (*fnam)[40] = precord->vall;
        char (*fdat)[40] = precord->valm;
        
        printf("Hello 1\n");
        for(i=0;i<32;i++){
            //memset(fnam[i], 0, 40);                 //Initialize the string to all zeros
            snprintf(fnam[i], 40, "%s", files[i]);  //Now copy the file names into the waveform
            //memset(fdat[i], 0, 40);
            snprintf(fdat[i], 40, "%s", dates[i]);  //Copy the file dates into the other waveform
        }
        printf("File Shift Ended...\n");
        memset(fnam[numfiles], 0, 40); 
        memset(fdat[numfiles], 0, 40); 
        precord->nevn = numfiles;
        memcpy((int *)precord->valn,fileID,32*sizeof(int));
    }
//    printf("End of COWsub\n");
    return(0);
}
// Note the function must be registered at the end!
epicsRegisterFunction(COWsub);

