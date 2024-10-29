#include "get_cluster.h"

uint32_t getNextCluster(const BootSector_t *bs, uint32_t startCluster)
{
    uint16_t nextCluster;
    uint32_t fatOffset = startCluster + (startCluster / 2);
    uint32_t fatSectorOffset = bs->reservedSectors * bs->bytesPerSector;

    HAL_fseek(fatSectorOffset + fatOffset);

    uint8_t fatEntry[2];

    if (HAL_fread(fatEntry, sizeof(uint8_t), 2) != 2)
    {
        return 0;
    }

    if (startCluster % 2 == 0)
    {
        nextCluster = (fatEntry[0] | (fatEntry[1] << 8)) & 0x0FFF;
    }
    else
    {
        nextCluster = ((fatEntry[0] >> 4) | (fatEntry[1] << 4)) & 0x0FFF;
    }

    if (nextCluster >= 0xFF8)
    {
        return 0;
    }

    return nextCluster;
}

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
    uint32_t startCluster,
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
            if (entryOut->name[0] != (char)0xE5)
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
    startCluster = getNextCluster(bs, startCluster);
    if (startCluster == 0)
    {
        return ERROR_INVALID_NAME;
    }
    else
    {
        findName(bs, filename, startCluster, entryOut, attribute);
    }
    return ERROR_OK;
}

error_code_t readFile(const BootSector_t *bs, DirectoryEntry_t *entry)
{
    uint16_t currentCluster = entry->startCluster;
    uint32_t bytesRead = 0;
    uint16_t sizeofBuffer = bs->bytesPerSector * bs->sectorsPerCluster;
    uint8_t buffer[sizeofBuffer + 1];

    while (currentCluster != 0)
    {
        uint32_t dataOffset = getAddressCluster(bs, currentCluster);
        HAL_fseek(dataOffset);
        size_t result = HAL_fread(buffer, 1, bs->bytesPerSector);

        if (result != bs->bytesPerSector)
        {
            printf("Error reading cluster\n");
            return -1;
        }

        int i;
        for (i = 0; i < bs->bytesPerSector && bytesRead < entry->fileSize; ++i)
        {
            printf("\033[0;31m%c\033[0m", buffer[i]);
            bytesRead++;
        }
        printf("\n");
        currentCluster = getNextCluster(bs, currentCluster);
    }

    return 0;
}

uint32_t findFreeCluster(const BootSector_t *bs)
{
    uint32_t fatSectorOffset = bs->reservedSectors * bs->bytesPerSector;
    HAL_fseek(fatSectorOffset);
    uint8_t fatEntry[3]; /* Read 3 bytes to handle 2 FAT12 entries */
    uint16_t buffer;
    uint16_t freeCluster;
    uint32_t totalClusters = (bs->sectorsPerFAT16 * bs->bytesPerSector * 2) / 3;
    for (freeCluster = 2; freeCluster < totalClusters; freeCluster++)
    {
        uint32_t fatOffset = (freeCluster * 3) / 2;
        uint32_t fatSector = bs->reservedSectors + (fatOffset / bs->bytesPerSector);
        uint32_t fatEntryOffset = fatOffset % bs->bytesPerSector;

        HAL_fseek(fatSector * bs->bytesPerSector + fatEntryOffset);
        HAL_fread(fatEntry, sizeof(uint8_t), 3);

        if (freeCluster % 2 == 0)
        {
            buffer = (fatEntry[1] << 8 | fatEntry[0]) & 0x0FFF;
        }
        else
        {
            buffer = (fatEntry[2] << 4 | fatEntry[1] >> 4) & 0x0FFF;
        }

        if (buffer == 0x000)
        {
            return freeCluster;
        }
    }

    return 0;
}

error_code_t markClusterUsed(uint32_t cluster, const BootSector_t *bs)
{
    uint32_t fatOffset;
    if (cluster % 2 == 0)
    {
        fatOffset = (cluster * 3) / 2;
    }
    else
    {
        fatOffset = ((cluster - 1) * 3) / 2;
    }

    HAL_fseek(bs->reservedSectors + fatOffset); // Write FAT1
    uint8_t fatEntry[3];                        /* Read 3 bytes to handle 2 FAT12 entries */
    HAL_fseek(bs->reservedSectors * bs->bytesPerSector + fatOffset);
    HAL_fread(fatEntry, sizeof(uint8_t), 3);

    if (cluster % 2 == 0)
    {
        fatEntry[0] = 0xFF;
        fatEntry[1] = (fatEntry[1] & 0xF0) | 0x0F;
    }
    else
    {
        fatEntry[1] = (fatEntry[1] & 0x0F) | 0xF0;
        fatEntry[2] = 0xFF;
    }

    HAL_fseek(bs->reservedSectors * bs->bytesPerSector + fatOffset);
    HAL_fwrite(fatEntry, sizeof(uint8_t), 3);

    return ERROR_OK;
}

uint8_t changeEntryFAT(uint16_t value, uint16_t startCluster, const BootSector_t *bs)
{
    uint32_t fatOffset = startCluster + (startCluster / 2);
    uint32_t fatSectorOffset = bs->reservedSectors * bs->bytesPerSector;

    HAL_fseek(fatSectorOffset + fatOffset);

    uint8_t fatEntry[2];

    if (HAL_fread(fatEntry, sizeof(uint8_t), 2) != 2)
    {
        return 1; // not ok
    }

    if (startCluster % 2 == 0)
    {
        fatEntry[0] = value & 0xFF;
        fatEntry[1] = (fatEntry[1] & 0xF0) | (value >> 8);
    }
    else
    {
        fatEntry[0] = (fatEntry[0] & 0x0F) | ((value & 0x0F) << 4);
        fatEntry[1] = value >> 4;
    }

    return 0; /// OK
}

error_code_t freeClusterUsed(uint32_t cluster, const BootSector_t *bs)
{
    uint32_t fatOffset;
    if (cluster % 2 == 0)
    {
        fatOffset = (cluster * 3) / 2;
    }
    else
    {
        fatOffset = ((cluster - 1) * 3) / 2;
    }

    HAL_fseek(bs->reservedSectors + fatOffset); // Write FAT1
    uint8_t fatEntry[3];                        /* Read 3 bytes to handle 2 FAT12 entries */
    HAL_fseek(bs->reservedSectors * bs->bytesPerSector + fatOffset);
    HAL_fread(fatEntry, sizeof(uint8_t), 3);

    if (cluster % 2 == 0)
    {
        fatEntry[0] = 0x00;
        fatEntry[1] = fatEntry[1] & 0xF0;
    }
    else
    {
        fatEntry[1] = fatEntry[1] & 0x0F;
        fatEntry[2] = 0x00;
    }

    HAL_fseek(bs->reservedSectors * bs->bytesPerSector + fatOffset);
    HAL_fwrite(fatEntry, sizeof(uint8_t), 3);

    return ERROR_OK;
}

error_code_t checkFolder(DirectoryEntry_t* entryOut)
{
    if (entryOut->name[0] == 0x00)
    {
        return ERROR_FILE_NOT_FOUND;
    }
    if (entryOut->name[0] != (char)0xE5)
    {
        if (entryOut->attr == ATTR_DIRECTORY)
        {
            return ERROR_OK;
        }
    }
    return ERROR_FILE_NOT_FOUND;
}

error_code_t checkFolderExist(const BootSector_t *bs,
    uint32_t startCluster,
    DirectoryEntry_t *entryOut)
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
            if (checkFolder(entryOut) == ERROR_OK)
            {
                error_code_t folderStatus = checkFolderExist(bs, entryOut->startCluster, entryOut);
                if (folderStatus == ERROR_OK)
                {
                    continue;
                }
            }
            else {
                continue;
            }
        }
    }
    startCluster = getNextCluster(bs, startCluster);
    if (startCluster == 0)
    {
        return ERROR_INVALID_NAME;
    }
    else
    {
        checkFolderExist(bs, startCluster, entryOut);
    }
    return ERROR_OK;
}