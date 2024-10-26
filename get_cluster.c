#include "get_cluster.h"

uint16_t getNextCluster(uint16_t startCluster, FILE *file)
{
    uint16_t nextAddress = startCluster;
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

uint16_t getAddressCluster(const BootSector_t *bs, uint16_t startcluster)
{
    uint32_t firstDataSector = bs->reservedSectors + (bs->numberOfFATs * bs->sectorsPerFAT16) + ((bs->rootEntryCount * 32 + (bs->bytesPerSector - 1)) / bs->bytesPerSector);
    uint32_t firstSectorOfCluster = ((startcluster - 2) * bs->sectorsPerCluster) + firstDataSector;

    uint32_t address = firstSectorOfCluster * bs->bytesPerSector;

    return address;    
    return 0;
}

error_code_t getEntry(FILE *fp, const BootSector_t *bs, DirectoryEntry_t *entryOut)
{
    
    return ERROR_OK;
}
error_code_t findName(FILE *fp, const BootSector_t *bs, char *filename, DirectoryEntry_t *entryOput)
{
    return ERROR_OK;
}
error_code_t readFile(FILE * file, uint16_t startCluster)
{
    return ERROR_OK;
}