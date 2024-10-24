#include "read_infor.h"

void read_boot_sector(FILE *fp, BootSector_t *bs)
{
    fseek(fp, 0, SEEK_SET);
    fread(bs, sizeof(BootSector_t), 1, fp);
}

void display_boot_sector_info(const BootSector_t *bs)
{
    printf("OEM Identifier: %.8s\n", bs->oem);
    printf("Bytes per sector: %u\n", bs->bytesPerSector);
    printf("Sectors per cluster: %u\n", bs->sectorsPerCluster);
    printf("Reserved sectors: %u\n", bs->reservedSectors);
    printf("Number of FATs: %u\n", bs->numberOfFATs);
    printf("Root entry count: %u\n", bs->rootEntryCount);
    printf("Total sectors (16-bit): %u\n", bs->totalSectors16);
    printf("Media descriptor: 0x%X\n", bs->mediaDescriptor);
    printf("Sectors per FAT (16-bit): %u\n", bs->sectorsPerFAT16);
    printf("Sectors per track: %u\n", bs->sectorsPerTrack);
    printf("Number of heads: %u\n", bs->numberOfHeads);
    printf("Hidden sectors: %u\n", bs->hiddenSectors);
    printf("Total sectors (32-bit): %u\n", bs->totalSectors32);
    printf("Sectors per FAT (32-bit): %u\n", bs->sectorsPerFAT32);
}