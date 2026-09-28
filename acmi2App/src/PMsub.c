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

    int process=0;
    double *PM = (double *)precord->a;
    int *status = (int *)precord->b;      // PM:Status-I (1=armed, 0=trigger_now, -1=frozen/triggered)
    int *faults_lat = (int *)precord->c;  // faults_lat-I (latched faults from FPGA)
    int *triggered = (int *)precord->d;   // PM:Triggered-I (only for UI display)
    
    // Check if faults have been cleared (by FPGA reset button or hardware reset)
    if (*faults_lat == 0 && *status == 2) {
        // Faults cleared and we were triggered -> re-arm for next fault
        *(int *)precord->vala = 1;    // Set status to 1 (armed, waveform released)
        *(int *)precord->valb = 0;    // Don't process
        *(int *)precord->valc = 0;    // Clear triggered flag for UI
        return(0);
    }
    
    // Check if armed (status = 1) and ready to detect new faults
    if (*status == 1) {
        // Check if faults are asserted
        int faultAsserted = ((PM[0] != 0.0) || (PM[1] != 0.0));
        
        if (faultAsserted) {
            // Fault detected -> transition to status=0 to trigger THIS cycle ONLY
            process = 1;
            *(int *)precord->vala = 0;  // Set status=0 (trigger this cycle)
            *(int *)precord->valb = 1;  // Set Process=1 to start capture
            *(int *)precord->valc = 1;  // Set Triggered=1 for UI
            return(0);
        }
        // No fault, stay armed
        *(int *)precord->vala = 1;
        *(int *)precord->valb = 0;
        *(int *)precord->valc = 0;
        return(0);
    }
    
    // If status=0, we just triggered - transition to status=-1 (frozen/triggered)
    // This ensures Process only fires once
    if (*status == 0) {
        // Fault detected last cycle, now freeze the waveform
            *(int *)precord->vala = 2;    // Freeze (status=2), waveform locked until reset
        *(int *)precord->valb = 0;    // Clear Process after this cycle
        *(int *)precord->valc = 1;    // Keep Triggered=1 for UI
        return(0);
    }
    
        // status=2: Frozen/triggered state, wait for faults to be cleared by reset button
        *(int *)precord->vala = 2;        // Stay frozen
    *(int *)precord->valb = 0;        // Never process again
    *(int *)precord->valc = 1;        // Keep showing triggered
    
    return(0);
}
// Note the function must be registered at the end!
epicsRegisterFunction(PMsub);
    
