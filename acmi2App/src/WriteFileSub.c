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

#define CRC32_INIT 0xFFFFFFFF //CRC Initial Value
#define CRC32_POLY 0x04C11DB7 //CRC Polynomial

// Function to compute CRC32 with 8-bit input
uint32_t crc32_update(uint32_t crc, uint8_t data) {
    crc ^= ((uint32_t)data << 24); // Align data to the highest byte

    for (int i = 0; i < 8; i++) { // Process 8 bits
        if (crc & 0x80000000) {   // Check MSB
            crc = (crc << 1) ^ CRC32_POLY;
        } else {
            crc <<= 1;
        }
    }
    return crc;
}

int WriteFileSub(aSubRecord *precord) {
    printf("Hello from WriteFileSub....\n");
    int i,Set[55];
    uint32_t crc= CRC32_INIT;
    unsigned char byteval;
    char names[50][40];

    int *TP1 = (int *)precord->a;
    int *TP2 = (int *)precord->b;
    int *TP3 = (int *)precord->c;
    int *BM = (int *)precord->d;
    
    FILE *fout = fopen("/home/diag/acmi2/settings/settings.txt","w");

    Set[0] = BM[9];		strcpy(names[0],"Settings Header");
    Set[1] = TP1[0];	strcpy(names[1],"TP1 Pulse Delay");
    Set[2] = TP1[1];	strcpy(names[2],"TP1 Pulse Width");
    Set[3] = TP1[4];	strcpy(names[3],"TP1 ADC Delay");
    Set[4] = TP2[0];	strcpy(names[4],"TP2 Pulse Delay");
    Set[5] = TP2[1];	strcpy(names[5],"TP2 Pulse Width");
    Set[6] = TP2[4];	strcpy(names[6],"TP2 ADC Delay");
    Set[7] = TP3[0];	strcpy(names[7],"TP3 Pulse Delay");
    Set[8] = TP3[1];	strcpy(names[8],"TP3 Pulse Width");
    Set[9] = TP3[4];	strcpy(names[9],"TP3 ADC Delay");
    Set[10] = BM[0];	strcpy(names[10],"Beam ADC Delay");
    Set[11] = BM[7];	strcpy(names[11],"COW Threshold");
    Set[12] = TP1[6];	strcpy(names[12],"TP1 Integral Low Level");
    Set[13] = TP1[5];	strcpy(names[13],"TP1 Integral High Level");
    Set[14] = TP2[6];	strcpy(names[14],"TP2 Integral Low Level");
    Set[15] = TP2[5];	strcpy(names[15],"TP2 Integral High Level");
    Set[16] = TP3[6];	strcpy(names[16],"TP3 Integral Low Level");
    Set[17] = TP3[5];	strcpy(names[17],"TP3 Integral High Level");
    Set[18] = TP1[8];	strcpy(names[18],"TP1 Peak Low Level");
    Set[19] = TP1[7];	strcpy(names[19],"TP1 Peak High Level");
    Set[20] = TP2[8];	strcpy(names[20],"TP2 Peak Low Level");
    Set[21] = TP2[7];	strcpy(names[21],"TP2 Peak High Level");
    Set[22] = TP3[8];	strcpy(names[22],"TP3 Peak Low Level");
    Set[23] = TP3[7];	strcpy(names[23],"TP3 Peak High Level");
    Set[24] = TP1[10];	strcpy(names[24],"TP1 FWHM Low Level");
    Set[25] = TP1[9];	strcpy(names[25],"TP1 FWHM High Level");
    Set[26] = TP2[10];	strcpy(names[26],"TP2 FWHM Low Level");
    Set[27] = TP2[9];	strcpy(names[27],"TP2 FWHM High Level");
    Set[28] = TP3[10];	strcpy(names[28],"TP3 FWHM Low Level");
    Set[29] = TP3[9];	strcpy(names[29],"TP3 FWHM High Level");
    Set[30] = TP1[12];	strcpy(names[30],"TP1 Baseline Low Level");
    Set[31] = TP1[11];	strcpy(names[31],"TP1 Baseline High Level");
    Set[32] = TP2[12];	strcpy(names[32],"TP2 Baseline Low Level");
    Set[33] = TP2[11];	strcpy(names[33],"TP2 Baseline High Level");
    Set[34] = TP3[12];	strcpy(names[34],"TP3 Baseline Low Level");
    Set[35] = TP3[11];	strcpy(names[35],"TP3 Baseline High Level");
    Set[36] = TP1[2];	strcpy(names[36],"TP1 Positive Level");
    Set[37] = TP2[2];	strcpy(names[37],"TP2 Positive Level");
    Set[38] = TP3[2];	strcpy(names[38],"TP3 Positive Level");
    Set[39] = TP1[3];	strcpy(names[39],"TP1 Negative Level");
    Set[40] = TP2[3];	strcpy(names[40],"TP2 Negative Level");
    Set[41] = TP3[3];	strcpy(names[41],"TP3 Negative Level");
    Set[42] = BM[6];	strcpy(names[42],"Accumulator High Level");
    Set[43] = BM[1];	strcpy(names[43],"Beam High Level");
    Set[44] = BM[3];	strcpy(names[44],"Beam Baseline Low Level");
    Set[45] = BM[2];	strcpy(names[45],"Beam Baseline High Level");
    Set[46] = BM[8];	strcpy(names[46],"Beam Charge Calibration");
    Set[47] = BM[4];	strcpy(names[47],"Accumulation Qmin");
    Set[48] = BM[5];	strcpy(names[48],"Accumulator Length");
    
    for (i=0;i<49;i++) {
//	    printf("Setting Num: %d: Value: %d\n",i,Set[i]);
        byteval = (Set[i] >> 24) & 0xFF; 
        crc = crc32_update(crc,byteval);
//    	    printf("Byte: %d   CRC32: 0x%08X\n",byteval,crc);
        byteval = (Set[i] >> 16) & 0xFF; 
	    crc = crc32_update(crc,byteval);
//	    printf("Byte: %d   CRC32: 0x%08X\n",byteval,crc);
        byteval = (Set[i] >> 8) & 0xFF; 
	    crc = crc32_update(crc,byteval);
//	    printf("Byte: %d   CRC32: 0x%08X\n",byteval,crc);
        byteval = (Set[i] >> 0 ) & 0xFF; 
	    crc = crc32_update(crc,byteval);
//	    printf("Byte: %d   CRC32: 0x%08X\n\n",byteval,crc);
    }
    Set[49] = crc;  strcpy(names[49],"File CRC Value");
    
    for(i=0;i<49;i++){
        printf("%d\t\t:\t%s\n",Set[i],names[i]);
        fprintf(fout,"%d\t\t:\t%s\n",Set[i],names[i]);
    }
    fprintf(fout,"%d\t\t:\t%s\n",Set[49],names[49]);
    printf("%d\t\t:\t%s\n",Set[49],names[49]);
    fclose(fout);   
    printf("CRC32 = 0x%x\n",crc);
    return(0);
}
// Note the function must be registered at the end!
epicsRegisterFunction(WriteFileSub);

