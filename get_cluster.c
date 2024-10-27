#include "get_cluster.h"

uint16_t getNextCluster(uint16_t startCluster)
{
    uint16_t nextAddress = startCluster;
    uint8_t fat[3]; // read 2 byte = 1 entry  +1/3 entry
    if (nextAddress % 2 == 0)
    {
        HAL_fseek(addressFAT + (3 * startCluster) / 2);
        HAL_fread(fat, sizeof(uint8_t), 3);
        nextAddress = (((fat[1] & 0x0F) << 8) | fat[0]);
    }
    else
    {
        HAL_fseek(addressFAT + (3 * (startCluster - 1)) / 2);
        HAL_fread(fat, sizeof(uint8_t), 3);
        nextAddress = (((fat[2] << 8) | fat[1]) >> 4);
    }
    // check next cluster
    if (nextAddress == 0x000U)
    {
        return FREE_CLUSTER;
    }
    else if (nextAddress == 0x001U)
    {
        return RESERVED_CLUSTER;
    }
    else if (nextAddress == 0xFF7U)
    {
        return BAD_CLUSTER;
    }
    else if ((nextAddress <= 0xFFFU) && (0xFF8U <= nextAddress))
    {
        return EOF_CLUSTER;
    }
    else
    {
        return nextAddress;
    }
}

/**
 * @brief Calculates the byte address of the first sector of a given cluster.
 *
 * This function computes the byte address of the first sector of a specified
 * cluster in a FAT file system. It uses the information from the boot sector
 * to determine the location.
 *
 * @param bs Pointer to the BootSector_t structure containing the boot sector information.
 * @param startCluster The cluster number for which the address is to be calculated.
 * @return The byte address of the first sector of the specified cluster.
 */
uint32_t getAddressCluster(const BootSector_t *bs, uint32_t startCluster)
{
    uint32_t firstDataSector = bs->reservedSectors + (bs->numberOfFATs * bs->sectorsPerFAT16) + ((bs->rootEntryCount * 32 + (bs->bytesPerSector - 1)) / bs->bytesPerSector);
    uint32_t firstSectorOfCluster = ((startCluster - 2) * bs->sectorsPerCluster) + firstDataSector;
    uint32_t address = firstSectorOfCluster * bs->bytesPerSector;
    return address;
}

error_code_t getEntry(const BootSector_t *bs, DirectoryEntry_t *entryOut)
{

    if (HAL_fread(entryOut, sizeof(DirectoryEntry_t), 1) != 1)
    {
        return ERROR_READ_FAILURE;
    }
    return ERROR_OK;
}

error_code_t findName(
    const BootSector_t *bs,
    char *filename,
    uint16_t startCluster,
    DirectoryEntry_t *entryOut,
    uint8_t attribute)
{
    HAL_fseek(getAddressCluster(bs, startCluster));
    int i;
    for (i = 0; i < bs->rootEntryCount; i++)
    {
        if (getEntry(bs, entryOut) != ERROR_OK)
        {
            return ERROR_INVALID_NAME;
        }
        else
        {
            if (entryOut->name[0] == 0x00)
            {
                continue;
            }
            if (entryOut->name[0] != 0xE5)
            {
                if (compareFileName(entryOut, filename))
                {
                    if (entryOut->attr == ATTR_DIRECTORY)
                    {
                        return attribute ? ERROR_OK : ERROR_WRONG_ATTRIBUTE;
                    }
                    else
                    {
                        return attribute ? ERROR_WRONG_ATTRIBUTE : ERROR_OK;
                    }
                }
            }
        }
    }
    startCluster = getNextCluster(startCluster);
    if (startCluster == FREE_CLUSTER ||
        startCluster == RESERVED_CLUSTER ||
        startCluster == BAD_CLUSTER ||
        ((startCluster >= 0xFF8) && (startCluster <= 0xFFF)))
    {
        return ERROR_INVALID_NAME;
    }
    else
    {
        findName(bs, filename, startCluster, entryOut, attribute);
    }
    return ERROR_OK;
}

error_code_t readFile(const BootSector_t *bs, uint16_t startCluster, DirectoryEntry_t *entry)
{
    uint32_t sizeOfCluster = bs->bytesPerSector * bs->sectorsPerCluster;
    uint32_t bytesRead = 0;
    uint32_t remainingBytes = entry->fileSize;

    HAL_fseek(getAddressCluster(bs, startCluster));
    while (remainingBytes > 0)
    {
        uint32_t i;
        for (i = 0; i < sizeOfCluster && remainingBytes > 0; i++)
        {
            int byte = HAL_fgetc();
            if (byte == EOF)
                break;
            printf("%c", (char)byte);
            bytesRead++;
            remainingBytes--;
        }

        if (remainingBytes > 0)
        {
            startCluster = getNextCluster(startCluster);
            if (startCluster == 0)
            {
                break;
            }
            else
            {
                HAL_fseek(getAddressCluster(bs, startCluster));
            }
        }
    }
    printf("\n");
    return ERROR_OK;
}
