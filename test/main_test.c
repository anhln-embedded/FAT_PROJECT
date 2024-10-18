#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#pragma pack(1)  // Đảm bảo cấu trúc không có padding

typedef struct {
    uint8_t jump[3];              // Nhảy qua code bootstrap
    char oem[8];                  // OEM Identifier
    uint16_t bytesPerSector;       // Số byte trên mỗi sector
    uint8_t sectorsPerCluster;     // Số sector trên mỗi cluster
    uint16_t reservedSectors;      // Số sector dự trữ (thường là 1)
    uint8_t numberOfFATs;          // Số lượng bảng FAT
    uint16_t rootEntryCount;       // Số mục trong thư mục gốc (FAT12/16)
    uint16_t totalSectors16;       // Tổng số sector (FAT12/16)
    uint8_t mediaDescriptor;       // Media type descriptor
    uint16_t sectorsPerFAT16;      // Số sector trên mỗi bảng FAT (FAT12/16)
    uint16_t sectorsPerTrack;      // Số sector trên mỗi track
    uint16_t numberOfHeads;        // Số đầu đọc của ổ đĩa
    uint32_t hiddenSectors;        // Số sector ẩn trước phân vùng này
    uint32_t totalSectors32;       // Tổng số sector (FAT32)
    uint32_t sectorsPerFAT32;      // Số sector trên mỗi bảng FAT (FAT32)
} BootSector;

void read_boot_sector(FILE *fp, BootSector *bs) {
    fseek(fp, 0, SEEK_SET);  // Di chuyển đến đầu file (Boot Sector)
    fread(bs, sizeof(BootSector), 1, fp);  // Đọc Boot Sector
}

void print_boot_sector_info(const BootSector *bs) {
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

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <image_file>\n", argv[0]);
        return 1;
    }

    FILE *fp = fopen(argv[1], "rb");
    if (fp == NULL) {
        perror("Error opening file");
        return 1;
    }

    BootSector bs;
    read_boot_sector(fp, &bs);
    print_boot_sector_info(&bs);

    fclose(fp);
    return 0;
}
