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
#include <errno.h>
#include <errlog.h>

#include <aSubRecord.h>
#include <registryFunction.h>
#include <epicsExport.h>

int COWFiles(aSubRecord *precord){
    printf("Hello from COWFiles....\n");

    /* --- validate index -------------------------------------------------- */
    int index = *(int *)precord->a;
    if (index < 0 || index >= 100) {
        errlogPrintf("COWFiles: FileIndex out of range: %d\n", index);
        return 0;
    }

    /* --- validate file ID ------------------------------------------------- */
    int *FileID = (int *)precord->b;
    if (FileID[index] <= 0) {
        errlogPrintf("COWFiles: FileID[%d]=%d is not valid (FileIDs-Wfm not yet populated?)\n",
                     index, FileID[index]);
        return 0;
    }

    /* --- build path safely (CHAR wfm is NOT null-terminated) -------------- */
    char path[64];
    memset(path, 0, sizeof(path));
    memcpy(path, (char *)precord->c, 60);   /* NOC=60, copy exactly that */
    if (path[0] == '\0') {
        errlogPrintf("COWFiles: COW:DirPath-SP is empty\n");
        return 0;
    }

    char fname[32];
    snprintf(fname, sizeof(fname), "COW%d.txt", FileID[index]);

    char fullname[256];
    size_t plen = strlen(path);
    if (path[plen - 1] == '/') {
        snprintf(fullname, sizeof(fullname), "%s%s", path, fname);
    } else {
        snprintf(fullname, sizeof(fullname), "%s/%s", path, fname);
    }
    printf("COWFiles: opening %s\n", fullname);

    /* --- write filename to output ----------------------------------------- */
    snprintf((char *)precord->vala, 80, "%s", fname);

    /* --- open file -------------------------------------------------------- */
    FILE *fp = fopen(fullname, "r");
    if (!fp) {
        errlogPrintf("COWFiles: Error opening %s: %s\n", fullname, strerror(errno));
        return 0;   /* return here – do NOT touch lines[] which is uninitialised */
    }

    /* --- read exactly 17 lines -------------------------------------------- */
    char lines[17][80];
    memset(lines, 0, sizeof(lines));
    int lnum = 0;
    while (lnum < 17 && fgets(lines[lnum], sizeof(lines[lnum]), fp) != NULL) {
        lines[lnum][sizeof(lines[lnum]) - 1] = '\0';
        lnum++;
    }
    fclose(fp);

    if (lnum < 17) {
        errlogPrintf("COWFiles: %s has only %d lines (expected 17)\n", fullname, lnum);
        return 0;
    }

    /* --- parse waveform (lines 0-15, 8 values each = 128 total) ----------- */
    int array[128];
    memset(array, 0, sizeof(array));
    int out_idx = 0;
    for (int j = 0; j < 16; j++) {
        char *saveptr = NULL;
        char *token = strtok_r(lines[j], ",", &saveptr);
        while (token != NULL && out_idx < 128) {
            array[out_idx++] = atoi(token);
            token = strtok_r(NULL, ",", &saveptr);
        }
    }
    if (out_idx != 128) {
        errlogPrintf("COWFiles: parsed %d waveform points from %s (expected 128)\n",
                     out_idx, fullname);
        return 0;
    }
    precord->nevb = 128;
    memcpy((int *)precord->valb, array, 128 * sizeof(int));

    /* --- parse stats line (line 16): Q_mc,intg,peak,fwhm,indx,base,... --- */
    int stats[6] = {0};
    int stat_idx = 0;
    char *saveptr = NULL;
    char *token = strtok_r(lines[16], ",", &saveptr);
    while (token != NULL && stat_idx < 6) {
        stats[stat_idx++] = atoi(token);
        token = strtok_r(NULL, ",", &saveptr);
    }
    if (stat_idx < 6) {
        errlogPrintf("COWFiles: only %d stats in %s (expected >=6)\n", stat_idx, fullname);
        return 0;
    }

    *(float *)precord->valc = (float)stats[0] / 1000.0f;  /* Q in nC */
    *(int *)precord->vald   = stats[1];                    /* intg    */
    *(int *)precord->vale   = stats[2];                    /* peak    */
    *(int *)precord->valf   = stats[3];                    /* fwhm    */
    *(int *)precord->valg   = stats[4];                    /* indx    */
    *(int *)precord->valh   = stats[5];                    /* base    */
    return 0;
}
 // Note the function must be registered at the end!
epicsRegisterFunction(COWFiles);

