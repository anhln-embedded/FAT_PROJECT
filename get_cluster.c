#include "get_cluster.h"

uint16_t getNextCluster(const BootSector_t *bs, uint16_t startCluster)
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

    // Kiểm tra nếu là FAT12 (tính toán khác cho FAT16 hoặc FAT32)
    if (startCluster % 2 == 0)
    {
        nextCluster = (fatEntry[0] | (fatEntry[1] << 8)) & 0x0FFF;
    }
    else
    {
        nextCluster = ((fatEntry[0] >> 4) | (fatEntry[1] << 4)) & 0x0FFF;
    }

    // Kiểm tra cluster tiếp theo có phải là cuối chuỗi không (theo chuẩn FAT12)
    if (nextCluster >= 0xFF8)
    {
        return 0; // Cuối chuỗi
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
            printf("%c", buffer[i]);
            bytesRead++;
        }
        printf("\n");
        currentCluster = getNextCluster(bs, currentCluster);
    }

    return 0;
}
