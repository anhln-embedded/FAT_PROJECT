#ifndef READ_FAT
#define READ_FAT

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define FAT12_ENTRY_SIZE 1.5            // Kích thước thực sự của mỗi entry là 1.5 byte
#define FAT12_TOTAL_BYTE_IN_FAT 9 * 512
#define STOP_READ_ENTRY 1

#define ADDRESS_FIRST_CLUSTER 123 /// ???

uint16_t readEntryFAT(uint16_t addressEntryFAT, FILE * file);
uint16_t fat2cluster(uint16_t addressEntryFAT, FILE * file);

#endif // CHECK_FAT
