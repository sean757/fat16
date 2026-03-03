#include "fat16.h"

uint16_t* task3(int fd, BootSector *bootsec) {
    uint32_t FATstart = bootsec->BPB_RsvdSecCnt * bootsec->BPB_BytsPerSec;
    uint32_t FATsize = bootsec->BPB_FATSz16 * bootsec->BPB_BytsPerSec;

    uint16_t *fat = malloc(FATsize);
    lseek(fd, FATstart, SEEK_SET);
    read(fd, fat, FATsize);

    printf("First 20 FAT entries: \n");
    for (int i=0; i<20; i++) {printf("FAT[%2d] = 0x%04X \n", i, fat[i]);}
    
    return fat;
};