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

int BMwriteSub(aSubRecord *precord)
// This subroutine takes the beam settings which were loaded from a file and updates
// the associated EPICS PVs with their values.  These PVs are configured to use the 
// PSC driver to pass the settings values to registers on the ARTIX FPGA.

// This subroutine is referenced in the "eeprom.db" EPICS database file.  The
// PV that uses this subroutine is "$(P){ACMI:$(NO)}BMwrite-Sub".
{
    printf("Hello from BMwriteSub....\n");  //for debugging

    int *A = (int *)precord->a;     //Settings for beam read from file

    *(int *)precord->vala = A[0];   //beam delay
    *(int *)precord->valb = A[1];   //beam high charge limit
    *(int *)precord->valc = A[2];   //beam baseline high limit
    *(int *)precord->vald = A[3];   //beam baseline low limit
    *(int *)precord->vale = A[4];   //accumulation low limit (not used)
    *(int *)precord->valf = A[5];   //accumulator trigger length
    *(int *)precord->valg = A[6];   //accumulator integrated charge high limit
    *(int *)precord->valh = A[7];   //charge outside window ADC threshold
    *(int *)precord->vali = A[8];   //charge calibration (integrated ADC counts per nC)
    *(int *)precord->valj = A[9];   //Settings header value
    *(int *)precord->valk = A[10];  //CRC value of the settings in the file

    return(0);
}
// Note the function must be registered at the end!
epicsRegisterFunction(BMwriteSub);
