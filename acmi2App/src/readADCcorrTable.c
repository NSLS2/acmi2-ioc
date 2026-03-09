#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <epicsExport.h>
#include <aSubRecord.h>
#include <registryFunction.h>


long readADCcorrTable(aSubRecord *prec) {
    const char *path = (const char *)prec->a;
    const char *filename = (const char *)prec->b;
    int maxElements = *(int *)prec->c;

    int32_t *outBuffer = (long *)prec->vala;
    long *numLines = (long *)prec->valb;

    if (!path || !filename || !outBuffer) {
        printf("Missing input(s)\n");
        return -1;
    }

    if (maxElements <= 0) {
        printf("Invalid maxElements %d\n", maxElements);
        return -1;
    }

    char fullPath[512];
    snprintf(fullPath, sizeof(fullPath), "%s/%s", path, filename);
    printf("Reading File: %s\n",fullPath);
    FILE *file = fopen(fullPath, "r");
    if (!file) {
        printf("Error: cannot open file %s\n", fullPath);
        return -1;
    }

    unsigned int count = 0;
    char line[128];
    while (fgets(line, sizeof(line), file) && count < (unsigned int)maxElements) {
        int val;
        if (sscanf(line, "%d", &val) == 1) {
            outBuffer[count++] = val;
            printf("Val: %d\n",val);
        } else {
            printf("Warning: invalid float in line %u: %s", count+1, line);
        }
    }
    fclose(file);

    if (count == 0) {
        printf("Warning: no valid values read from %s\n", fullPath);
        *numLines = 0;
        return -1;
    }
    printf("Read %d Values\n",(int)count);

    prec->neva = count;
    *numLines = count;

    printf("Read %u float values from %s\n", count, fullPath);
    return 0;
}

epicsRegisterFunction(readADCcorrTable);



