#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#pragma pack(1)  // Đảm bảo không có padding

typedef struct {
    uint8_t jump[3];              
    char oem[8];                  
    uint16_t bytesPerSector;       
    uint8_t sectorsPerCluster;     
    uint16_t reservedSectors;      
    uint8_t numberOfFATs;          
    uint16_t rootEntryCount;       
    uint16_t totalSectors16;       
    uint8_t mediaDescriptor;       
    uint16_t sectorsPerFAT16;      
    uint16_t sectorsPerTrack;      
    uint16_t numberOfHeads;        
    uint32_t hiddenSectors;        
    uint32_t totalSectors32;       
    uint32_t sectorsPerFAT32;      
} BootSector;

typedef struct {
    char name[11];                // Tên tệp
    uint8_t attr;                 // Thuộc tính
    uint8_t reserved[10];         // Dữ liệu dự trữ
    uint16_t time;                // Thời gian
    uint16_t date;                // Ngày
    uint16_t startCluster;        // Số cluster đầu tiên
    uint32_t fileSize;            // Kích thước tệp
} DirectoryEntry;

void read_boot_sector(FILE *fp, BootSector *bs) {
    fseek(fp, 0, SEEK_SET);
    if (fread(bs, sizeof(BootSector), 1, fp) != 1) {
        perror("Error reading boot sector");
        exit(EXIT_FAILURE);
    }
}

void read_root_directory(FILE *fp, const BootSector *bs) {
    uint32_t rootDirSector = bs->reservedSectors + (bs->numberOfFATs * bs->sectorsPerFAT16);
    uint32_t rootDirSize = ((bs->rootEntryCount * sizeof(DirectoryEntry)) + (bs->bytesPerSector - 1)) / bs->bytesPerSector;
    uint32_t rootDirStart = rootDirSector * bs->bytesPerSector;

    fseek(fp, rootDirStart, SEEK_SET);

    int i;  // Khai báo biến trước vòng for
    for (i = 0; i < bs->rootEntryCount; i++) {
        DirectoryEntry entry;
        if (fread(&entry, sizeof(DirectoryEntry), 1, fp) != 1) {
            perror("Error reading directory entry");
            exit(EXIT_FAILURE);
        }

        // Kiểm tra xem mục có hợp lệ không (bắt đầu bằng dấu NULL)
        if (entry.name[0] == 0x00) {
            break;  // Đến cuối thư mục
        }

        // In thông tin tệp
        if (entry.name[0] != 0xE5) { // 0xE5 nghĩa là mục đã bị xóa
            printf("File Name: %.8s.%.3s\n", entry.name, entry.name + 8);
            printf("Size: %u bytes\n", entry.fileSize);
            printf("Starting Cluster: %u\n", entry.startCluster);
            printf("Attributes: 0x%X\n", entry.attr);
            printf("------------------------------\n");
        }
    }
}

int main(int argc, char *argv[]) {
    // if (argc != 2) {
    //     fprintf(stderr, "Usage: %s <image_file>\n", argv[0]);
    //     return EXIT_FAILURE;
    // }

    // FILE *fp = fopen(argv[1], "rb");
    // if (fp == NULL) {
    //     perror("Error opening file");
    //     return EXIT_FAILURE;
    // }

    // BootSector bs;
    // read_boot_sector(fp, &bs);
    // read_root_directory(fp, &bs);

    // fclose(fp);
    // return EXIT_SUCCESS;

    uint16_t RawData = 0x3039;
     result
}
