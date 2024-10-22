#include "read_FAT.h"

uint16_t readEntryFAT(uint16_t addressEntryFAT, FILE * file)
{
    uint16_t nextAddress;
    uint8_t fat[2];                                 // read 2 byte = 1 entry  +1/3 entry
    fseek(file, addressEntryFAT, SEEK_SET);         // come to address FAT
    fread(fat, sizeof(uint8_t), 2, file);           // read 2 bytes

    // solve entry 1 2
    //little endieness
    if (addressEntryFAT % 2 ==0)
    {
        nextAddress = (fat[0] | ((fat[1] & 0x0F) << 8));
    }
    else
    {
        nextAddress = (fat[0] >> 4) | (fat[1] << 4);
    }

       // kiểm tra điều kiện

    if(nextAddress == 0x000)
        {
            printf("cluster's empty");
            return STOP_READ_ENTRY;
        }
    else if(nextAddress <= 0xFFF && nextAddress >= 0xFF8)
        {
            printf("EOF");
            return STOP_READ_ENTRY;
        }
    else if(nextAddress <= 0xFF6 && nextAddress >= 0xFF0)
        {
            printf("Reserved Values");
            return STOP_READ_ENTRY;
        }
    else if (nextAddress== 0xFF7)
        {
            printf("cluster error");
            return STOP_READ_ENTRY;
        }
    else
    {
        return nextAddress;
    }
}


uint16_t fat2cluster(uint16_t addressEntryFAT, FILE * file)
{
    return readEntryFAT(addressEntryFAT, file) + ADDRESS_FIRST_CLUSTER;
}