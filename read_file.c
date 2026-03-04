#include "fat16.h"

void task5(int fd, BootSector *bootsec, uint16_t *fat, Directory *entries) {
    Directory *txtFile = NULL;
    for (int i = 0; i < 64; i++) {
        if (entries[i].DIR_Name[0] == 0x00) break;
        if (entries[i].DIR_Name[0] == 0xE5) continue;
        if (entries[i].DIR_Attr == 0x0F) continue;
        if (entries[i].DIR_Attr & 0x10) continue; 
        
        if (strncmp((char *)&entries[i].DIR_Name[8], "TXT", 3) == 0) {
            txtFile = &entries[i];
            break;
        }
    }

    if (txtFile == NULL) {
        printf("No .txt file found\n");
        return;
    }

    uint16_t cluster = txtFile->DIR_FstClusLO;
    uint32_t fileSize = txtFile->DIR_FileSize;

    printf("Reading file: %.11s\n", txtFile->DIR_Name);
    printf("Size: %u bytes, Starting cluster: %u\n", fileSize, cluster);

    uint32_t rootStart = (bootsec->BPB_RsvdSecCnt + bootsec->BPB_NumFATs * bootsec->BPB_FATSz16) * bootsec->BPB_BytsPerSec;
    uint32_t rootSize = bootsec->BPB_RootEntCnt * 32;
    uint32_t dataStart = rootStart + rootSize;
    uint32_t bytesPerCluster = bootsec->BPB_SecPerClus * bootsec->BPB_BytsPerSec;

    printf("Data region starts at byte: %u\n", dataStart);
    printf("Bytes per cluster: %u\n", bytesPerCluster);

    uint32_t clusterPos = dataStart + (cluster - 2) * bytesPerCluster;
    printf("Cluster %d is at byte: %u\n", cluster, clusterPos);

    char *buffer = malloc(fileSize + 1);
    lseek(fd, clusterPos, SEEK_SET);
    read(fd, buffer, fileSize);
    buffer[fileSize] = '\0';

    printf("Content: %s\n", buffer);

    printf("FAT chain: ");
    uint16_t currentCluster = cluster;
    while (currentCluster < 0xFFF8) {
        printf("%d", currentCluster);
        currentCluster = fat[currentCluster];
        if (currentCluster < 0xFFF8) printf(" -> ");
    }
    printf("\n\n");

    free(buffer);
}