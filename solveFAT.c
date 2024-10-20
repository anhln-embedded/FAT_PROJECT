#include "check_fat.h"

uint16_t readEntry(uint16_t addressEntry, FILE * file)
{
    uint16_t nextAddress;
    uint8_t fat[2];                     // read 2 byte = 1 entry  +1/3 entry
    fseek(file, addressEntry, SEEK_SET);       // come to address FAT
    fread(fat, sizeof(uint8_t), 2, file);         // read 2 bytes

    // solve entry 1 2
    //little endieness
    if (addressEntry % 2 ==0)
    {
        nextAddress = (fat[0] | ((fat[1] & 0x0F) << 8));
    }
    else
    {
        nextAddress = (fat[0] >> 4) | (fat[1] << 4);
    }

    // function read data are


    return nextAddress + 0x200;
}
