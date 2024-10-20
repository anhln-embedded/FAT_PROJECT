#include "check_fat.h"

void read_fat_table(FILE *file)
{
    fseek(file, 0x200, SEEK_SET);       // come to address FAT
    uint8_t fat[3];                     // read 3 byte = 2 entry

    printf("FAT12 Table:\n");
    for (uint16_t i = 0; i < FAT12_TOTAL_BYTE_IN_FAT; i += 2)
    {
        fread(fat, sizeof(uint8_t), 3, file);         // read 3 byte (2 entry)

        // solve entry 1 2
        //little endieness
        uint16_t entry1 = (fat[0] | ((fat[1] & 0x0F) << 8)); // Entry first
        uint16_t entry2 = ((fat[1] >> 4) | (fat[2] << 4)); // Entry second
        // print

        printf("entry %d: %03X\n", i, entry1);

        if (i + 1 < FAT12_TOTAL_BYTE_IN_FAT)
        {
            printf("entry %d: %03X\n", i + 1, entry2);
        }
    }
}
