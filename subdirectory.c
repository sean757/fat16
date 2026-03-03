#include "fat16.h"

void task7(int fd, BootSector *bootsec, uint16_t *fat){
    uint32_t rootStart = (bootsec->BPB_RsvdSecCnt + bootsec->BPB_NumFATs * bootsec->BPB_FATSz16) * bootsec->BPB_BytsPerSec;
    uint32_t rootSize = bootsec->BPB_RootEntCnt * 32;
    uint32_t dataStart = rootStart + rootSize;
    uint32_t bytesPerCluster = bootsec->BPB_SecPerClus * bootsec->BPB_BytsPerSec;
    uint16_t manCluster = 4;
    uint32_t manPos = dataStart + (manCluster - 2) * bytesPerCluster;

    lseek(fd, manPos, SEEK_SET);
    Directory manEntries[64];
    read(fd, manEntries, bytesPerCluster);

    printf("MAN Directory contents: \n");
    for (int i=0; i<32; i++) {
        if (manEntries[i].DIR_Name[0] == 0x00) break;
        if (manEntries[i].DIR_Name[0] == 0xE5) continue;
        if (manEntries[i].DIR_Attr == 0x0F) continue;
        printf("%.11s\n", manEntries[i].DIR_Name);

        if (strncmp((char *)manEntries[i].DIR_Name, "MAN2", 4) == 0) {
            printf("MAN2 Directory information: \nSize: %u, Cluster: %u \n", manEntries[i].DIR_FileSize, manEntries[i].DIR_FstClusLO);
        }
    }

    uint16_t man2Cluster = 5;
    uint32_t man2Pos = dataStart + (man2Cluster - 2) * bytesPerCluster;

    lseek(fd, man2Pos, SEEK_SET);
    Directory man2Entries[64];
    read(fd, man2Entries, bytesPerCluster);
    
    printf("MAN2 Directory contents: \n");
    for (int i=0; i<32; i++) {
        if (man2Entries[i].DIR_Name[0] == 0x00) break;
        if (man2Entries[i].DIR_Name[0] == 0xE5) continue;
        if (man2Entries[i].DIR_Attr == 0x0F) continue;
        printf("%.11s\n", man2Entries[i].DIR_Name);
    }
};