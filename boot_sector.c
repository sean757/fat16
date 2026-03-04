#include "fat16.h"

void task2(int fd, BootSector *bootsec) {
    
    lseek(fd, 0, SEEK_SET);
    read(fd, bootsec, sizeof(BootSector));

    printf("Bytes per sector: %d\n", bootsec->BPB_BytsPerSec);
    printf("Sectors per cluster: %d\n", bootsec->BPB_SecPerClus);
    printf("Reserved sectors: %d\n", bootsec->BPB_RsvdSecCnt);
    printf("Number of FATs: %d\n", bootsec->BPB_NumFATs);
    printf("Root entry count: %d\n", bootsec->BPB_RootEntCnt);
    printf("Total sectors (16): %u\n", bootsec->BPB_TotSec16);
    printf("FAT size (sectors): %d\n", bootsec->BPB_FATSz16);
    printf("Total sectors (32): %u\n", bootsec->BPB_TotSec32);
    printf("Volume label: %.11s\n", bootsec->BS_VolLab);
    printf("\n");
}