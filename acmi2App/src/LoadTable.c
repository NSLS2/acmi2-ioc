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
#include <errno.h>
#include <sys/stat.h>

#include <aSubRecord.h>
#include <registryFunction.h>
#include <epicsExport.h>

int32_t LoadTable(aSubRecord *precord) {
    printf("Hello from LoadTable....\n");

    int *Table = (int *)precord->a;
    int tm = *(int *)precord->b;
    char DirPath[128] = {0};
    int apply = *(int *)precord->d;
    
    char fname[160];
    int i,j;

    memcpy(DirPath, (char *)precord->c, 60);
    DirPath[60] = '\0';
    
    memcpy((int32_t *)precord->vala,Table,16000*sizeof(int32_t));
    snprintf(fname, sizeof(fname), "%sCorrection%d.txt", DirPath, tm);
    snprintf((char *)precord->valb, 100, "%s", fname);
    
    if(apply==1){
        printf("filename = %s\n",fname);

        if (mkdir(DirPath, 0775) != 0 && errno != EEXIST) {
            perror("LoadTable mkdir");
            return 0;
        }
        
        FILE *fout = fopen(fname,"w");
        if (fout == NULL) {
            perror("LoadTable fopen");
            return 0;
        }
    
        for(i=0;i<1000;i=i+1){
            for(j=0;j<15;j=j+1){
                fprintf(fout,"%d,",Table[i*16+j]);
            }
            fprintf(fout,"%d\n",Table[i*16+15]);
        }
        fclose(fout); 
    }

    return(0);
}
// Note the function must be registered at the end!
epicsRegisterFunction(LoadTable);
