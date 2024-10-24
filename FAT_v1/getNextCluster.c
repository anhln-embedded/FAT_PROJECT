#include "getNextCluster.h"

START_CLUSTER getNextCluster(START_CLUSTER startCluster, FILE *file)
{

    uint16_t nextAddress;
    uint8_t fat[3]; // read 2 byte = 1 entry  +1/3 entry
    if (nextAddress % 2 == 0)
    {
        fseek(file, addressFAT + 1.5 * startCluster, SEEK_SET); // come to address FAT
        fread(fat, sizeof(uint8_t), 3, file);                   // read 2 bytes
        nextAddress = (fat[1] << 8 | fat[0]) & 0x0FFF;
    }
    else
    {
        fseek(file, addressFAT + 1.5 * (startCluster - 1), SEEK_SET); // come to address FAT
        fread(fat, sizeof(uint8_t), 3, file);                         // read 2 bytes
        nextAddress = (fat[2] << 4 | fat[1] >> 4) & 0x0FFF;
    }

    // check next cluster

    if (nextAddress == 0x000)
    {
        return FREE_CLUSTER;
    }
    else if (nextAddress == 0x001)
    {
        return RESERVED_CLUSTER;
    }
    else if (nextAddress == 0xFF7)
    {
        return BAD_CLUSTER;
    }
    else if (nextAddress <= 0xFFF && nextAddress >= 0xFF8)
    {
        return EOF_CLUSTER;
    }
    else
    {
        return nextAddress;
    }
}
