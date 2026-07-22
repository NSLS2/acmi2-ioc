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

int PMsub(aSubRecord *precord) {
//    printf("Hello from PMSub....\n");

    int armed = *(int *)precord->b;
    int process=0;
    double *PM = (double *)precord->a;
    // PM:Fault-Wfm is a 2-sample circular buffer; sample order can vary.
    // Trigger once when faults transition from clear to asserted.
    int faultAsserted = ((PM[0] != 0.0) || (PM[1] != 0.0));

    if (!faultAsserted) {
        armed = 1;
    } else if (armed) {
        armed = 0;
        process = 1;
    }
    
    *(int *)precord->vala = armed;
    *(int *)precord->valb = process;
    return(0);
}
// Note the function must be registered at the end!
epicsRegisterFunction(PMsub);
    
