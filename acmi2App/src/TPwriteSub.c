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

int TPwriteSub(aSubRecord *precord)
// This subroutine takes the TP settings which were loaded from a file and updates
// the associated EPICS PVs with their values.  These PVs are configured to use the 
// PSC driver to pass the settings values to registers on the ARTIX FPGA.

// This subroutine is referenced in the "eeprom.db" EPICS database file.  The
// PVs that uses this subroutine are "$(P){ACMI:$(NO)}TP1write-Sub", 
// "$(P){ACMI:$(NO)}TP2write-Sub", "$(P){ACMI:$(NO)}TP1write-Sub".
{
    printf("Hello from TPwriteSub....\n");  //for debugging

    int zero=0;
    int *A = (int *)precord->a;     //Array containing 13 settings for the TP

    *(int *)precord->vala = A[0];   //TP pulse delay
    *(int *)precord->valb = A[1];   //TP pulse width
    *(int *)precord->valc = A[2];   //TP positve level
    *(int *)precord->vald = A[3];   //TP negative level
    *(int *)precord->vale = A[4];   //TP ADC delay
    *(int *)precord->valf = A[5];   //TP charge integral high limit
    *(int *)precord->valg = A[6];   //TP charge integral low limit
    *(int *)precord->valh = A[7];   //TP ADC peak high limit
    *(int *)precord->vali = A[8];   //TP ADC peak low limit
    *(int *)precord->valj = A[9];   //TP FWHM high limit
    *(int *)precord->valk = A[10];  //TP FWHM low limit
    *(int *)precord->vall = A[11];  //TP baseline high limit
    *(int *)precord->valm = A[12];  //TP baseline low limit

    *(int *)precord->valn = zero;  //Clear the command to load settings

    return(0);
}
// Note the function must be registered at the end!
epicsRegisterFunction(TPwriteSub);
