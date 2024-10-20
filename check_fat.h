#ifndef CHECK_FAT
#define CHECK_FAT

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define FAT12_ENTRY_SIZE 1.5            // Kích thước thực sự của mỗi entry là 1.5 byte
#define FAT12_TOTAL_BYTE_IN_FAT 9*512

uint16_t readEntry(uint16_t addressEntry, FILE * file);

void read_fat_table(FILE *file);

#endif // CHECK_FAT
