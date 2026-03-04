#include "fat16.h"

Directory* task4_6(int fd, BootSector *bootsec, uint16_t *fat) {
    uint32_t rootStart = (bootsec->BPB_RsvdSecCnt + bootsec->BPB_NumFATs * bootsec->BPB_FATSz16) * bootsec->BPB_BytsPerSec;
    printf("Root directory starts at byte: %d\n", rootStart);

    lseek(fd, rootStart, SEEK_SET);
    Directory *entries = malloc(64 * sizeof(Directory));
    read(fd, entries, 64 * sizeof(Directory));

    printf("Directory entries: \n");
    char longName[256] = "";
    
    for (int i=0; i<64; i++) {
        if (entries[i].DIR_Name[0] == 0x00) break;
        if (entries[i].DIR_Name[0] == 0xE5) continue;
        if (entries[i].DIR_Attr == 0x0F) {
            LongDirectory *longDir = (LongDirectory *)&entries[i];

            char part[14];
            int m = 0;

            for (int n=0; n<10; n+=2) {
                if (longDir->LDIR_Name1[n] == 0xFF) break;
                part[m++] = longDir->LDIR_Name1[n];
            }
            for (int n=0; n<12; n+=2) {
                if (longDir->LDIR_Name2[n] == 0xFF) break;
                part[m++] = longDir->LDIR_Name2[n];
            }
            for (int n=0; n<4; n+=2) {
                if (longDir->LDIR_Name3[n] == 0xFF) break;
                part[m++] = longDir->LDIR_Name3[n];
            }
            part[m] = '\0';

            char temp[256];
            strcpy(temp, longName);
            strcpy(longName, part);
            strcat(longName, temp);

            continue;
        }

        char attr[7];
        attr[0] = (entries[i].DIR_Attr & 0x20) ? 'A' : '-';
        attr[1] = (entries[i].DIR_Attr & 0x10) ? 'D' : '-';
        attr[2] = (entries[i].DIR_Attr & 0x08) ? 'V' : '-';
        attr[3] = (entries[i].DIR_Attr & 0x04) ? 'S' : '-';
        attr[4] = (entries[i].DIR_Attr & 0x02) ? 'H' : '-';
        attr[5] = (entries[i].DIR_Attr & 0x01) ? 'R' : '-';
        attr[6] = '\0';

        int year = ((entries[i].DIR_WrtDate >> 9) & 0x7F) + 1980;
        int month = (entries[i].DIR_WrtDate >> 5) & 0x0F;
        int day = entries[i].DIR_WrtDate & 0x1F;
        int hour = (entries[i].DIR_WrtTime >> 11) & 0x1F;
        int minute = (entries[i].DIR_WrtTime >> 5) & 0x3F;
        int second = (entries[i].DIR_WrtTime & 0x1F) * 2;

        if (longName[0] != '\0') {
            printf("%s %s %04d-%02d-%02d %02d:%02d:%02d Size: %d Cluster: %d\n",
                   attr, longName, year, month, day, hour, minute, second, entries[i].DIR_FileSize, entries[i].DIR_FstClusLO);
        } else {
            printf("%s %.11s %04d-%02d-%02d %02d:%02d:%02d Size: %d Cluster: %d\n",
                   attr, entries[i].DIR_Name, year, month, day, hour, minute, second, entries[i].DIR_FileSize, entries[i].DIR_FstClusLO);
        }

        longName[0] = '\0';
    }

    return entries;
}