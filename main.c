
#include "fat16.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <fat16_image.img>\n", argv[0]);
        return 1;
    }

    int fd = open(argv[1], O_RDONLY);
    if (fd < 0) {
        perror("Error opening image file");
        return 1;
    }

    BootSector bootsec;

    printf("FAT16 Image File: %s\n", argv[1]);

    task2(fd, &bootsec);
    uint16_t *fat = task3(fd, &bootsec);
    Directory *entries = task4_6(fd, &bootsec, fat);
    task5(fd, &bootsec, fat, entries);
    task7(fd, &bootsec, fat);   

    free(fat);
    close(fd);
    return 0;
}