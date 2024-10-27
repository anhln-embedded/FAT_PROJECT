#include "get_cluster.h"

uint16_t getNextCluster(uint16_t startCluster, FILE *file)
{
    uint16_t nextAddress = startCluster;
    uint8_t fat[3]; // read 2 byte = 1 entry  +1/3 entry
    if (nextAddress % 2 == 0)
    {
        fseek(file, (int)(addressFAT + (3 * startCluster) / 2), SEEK_SET); // come to address FAT
        fread(fat, sizeof(uint8_t), 3, file);
        nextAddress = (((fat[1] & 0x0F) << 8) | fat[0]);
    }
    else
    {
        fseek(file, (int)(addressFAT + (3 * (startCluster - 1)) / 2), SEEK_SET); // come to address FAT
        fread(fat, sizeof(uint8_t), 3, file);
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

uint32_t getAddressCluster(const BootSector_t *bs, uint32_t startCluster)
{
    // Tính toán vị trí của sector đầu tiên của cluster
    uint32_t firstDataSector = bs->reservedSectors + (bs->numberOfFATs * bs->sectorsPerFAT16) + ((bs->rootEntryCount * 32 + (bs->bytesPerSector - 1)) / bs->bytesPerSector);
    uint32_t firstSectorOfCluster = ((startCluster - 2) * bs->sectorsPerCluster) + firstDataSector;

    // Chuyển đổi vị trí sector thành địa chỉ byte trong tệp
    uint32_t address = firstSectorOfCluster * bs->bytesPerSector;
    return address;
}

error_code_t getEntry(FILE *fp, const BootSector_t *bs, DirectoryEntry_t *entryOut)
{
    if (fread(entryOut, sizeof(DirectoryEntry_t), 1, fp) != 1) // ask later
    {
        return ERROR_READ_FAILURE;
    }
    return ERROR_OK;
}

error_code_t findName(FILE *fp, const BootSector_t *bs, char *filename, uint16_t startCluster, DirectoryEntry_t *entryOut)
{
    fseek(fp, getAddressCluster(bs, startCluster), SEEK_SET);
    int i;
    for (i = 0; i < bs->rootEntryCount; i++)
    {
        if (getEntry(fp, bs, entryOut) != ERROR_OK)
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
                    return ERROR_OK;
                }
            }
        }
    }
    startCluster = getNextCluster(startCluster, fp);
    if (startCluster == FREE_CLUSTER ||
        startCluster == RESERVED_CLUSTER ||
        startCluster == BAD_CLUSTER ||
        ((startCluster >= 0xFF8) && (startCluster <= 0xFFF)))
    {
        return ERROR_INVALID_NAME;
    }
    else
    {
        findName(fp, bs, filename, startCluster, entryOut);
    }
    return ERROR_OK;
}

error_code_t readFile(FILE *fp, const BootSector_t *bs, uint16_t startCluster, DirectoryEntry_t *entry)
{
    uint32_t sizeOfCluster = bs->bytesPerSector * bs->sectorsPerCluster;
    uint32_t bytesRead = 0;
    uint32_t remainingBytes = entry->fileSize;

    fseek(fp, getAddressCluster(bs, startCluster), SEEK_SET);
    while (remainingBytes > 0)
    {
        uint32_t i;
        for ( i = 0; i < sizeOfCluster && remainingBytes > 0; i++)
        {
            int byte = fgetc(fp);
            if (byte == EOF) break;
            printf("%c", (char)byte);
            bytesRead++;
            remainingBytes--;
        }

        if (remainingBytes > 0)
        {
            startCluster = getNextCluster(startCluster, fp);
            if (startCluster == 0)
            {
                break;
            }
            else
            {
                fseek(fp, getAddressCluster(bs, startCluster), SEEK_SET);
            }
        }
    }
    printf("\n");
    return ERROR_OK;
}

/*
error_code_t readFile(FILE *fp, const BootSector_t *bs, uint16_t startCluster, DirectoryEntry_t *entry)
{
    fseek(fp, getAddressCluster(bs, startCluster), SEEK_SET);
    uint16_t sizeOfCluster = 1 + sizeof(char) * bs->bytesPerSector * bs->sectorsPerCluster;
    char *buffContent;
    if (entry->fileSize <= sizeOfCluster)
    {
        buffContent = (char *)malloc(sizeOfCluster + 1);
        fread(buffContent, 1, sizeOfCluster, fp);
        buffContent[sizeOfCluster] = '\0';
        printf("%s", buffContent);
        free(buffContent);
    }
    else
    {
        // for (counter = 0; counter < (entry->fileSize / sizeOfCluster); counter += 1)
        while (1)
        {
            buffContent = (char *)malloc(sizeOfCluster + 1);
            fread(buffContent, 1, sizeOfCluster, fp);
            buffContent[sizeOfCluster] = '\0';
            printf("%s", buffContent);
            free(buffContent);

            startCluster = getNextCluster(startCluster, fp);
            if (startCluster == FREE_CLUSTER ||
                startCluster == RESERVED_CLUSTER ||
                startCluster == BAD_CLUSTER ||
                ((startCluster >= 0xFF8) && (startCluster <= 0xFFF)))
            {
                break;
            }
            else
            {
                fseek(fp, getAddressCluster(bs, startCluster), SEEK_SET);
            }
        }
    }

    return ERROR_OK;
}

*/