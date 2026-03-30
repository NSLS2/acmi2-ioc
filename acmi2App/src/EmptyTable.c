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

int EmptyTable(aSubRecord *precord) {
    printf("Hello from EmptyTable....\n");

    int Table[16000] = {0};

    memcpy((int32_t *)precord->vala,Table,16000*sizeof(int32_t));
    memcpy((int32_t *)precord->valb,Table,16000*sizeof(int32_t));

    return(0);
}
// Note the function must be registered at the end!
epicsRegisterFunction(EmptyTable);
