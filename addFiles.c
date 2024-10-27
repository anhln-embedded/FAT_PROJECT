
#include "addFiles.h"

uint32_t getSizeFile(const char *filePath)
{
    FILE *file = fopen(filePath, "rb");
    if (file == NULL)
    {
        printf("Do not open file");
        return 1; // notes
    }
    fseek(file, 0, SEEK_END);
    uint32_t fileSize = ftell(file);
    fclose(file);
    return fileSize;
}

uint16_t findTempEntryFat(FILE *file, const BootSector_t *bs)
{
    fseek(file, bs->reservedSectors * bs->bytesPerSector, SEEK_SET);
    uint16_t counter;
    uint16_t tempEntry;
    for (counter = 0; counter < (bs->sectorsPerFAT16 * bs->bytesPerSector); counter++)
    {
        if (getNextCluster(counter, file) == FREE_CLUSTER)
        {
            break;
        }

        if (counter == bs->sectorsPerFAT16 * bs->bytesPerSector - 1)
        {
            return 1; // notes --- don't file temp cluster
        }
    }
    return counter;
}

uint8_t writeContent(FILE *file, const BootSector_t *bs, uint32_t addressCluster, const char *filePath, uint32_t fileSize)
{
    fseek(file, addressCluster, SEEK_SET);
    char *buffer = (char *)malloc(fileSize);
    if (!buffer)
    {
        return 1; /// error malloc
    }

    FILE *filePathOpen = fopen(filePath, "rb");
    uint32_t bytesRead = fread(buffer, 1, bs->bytesPerSector * bs->sectorsPerCluster, filePathOpen);
    if (!bytesRead)
    {
        free(buffer);
        fclose(filePathOpen);
        return 1; // error read
    }

    uint32_t bytesWrite = fwrite(buffer, 1, bytesRead, file);
    if (bytesRead != bytesWrite)
    {
        printf("\nError writing file");
        free(buffer);
        fclose(filePathOpen);
        return 1;
    }

    free(buffer);
    fclose(filePathOpen);
    return 0; /// OK
}

// get file's name
char *getFileName(const char *filePath)
{
    const char *lastSlash = strrchr(filePath, '/');
    if (lastSlash == NULL)
    {
        lastSlash = strrchr(filePath, '\\');
    }

    const char *fileStart = (lastSlash != NULL) ? lastSlash + 1 : filePath;
    const char *dot = strchr(fileStart, '.');
    size_t fileNameLength = (dot != NULL) ? (dot - fileStart) : strlen(fileStart);
    char *fileName = (char *)malloc(fileNameLength + 1);
    if (fileName != NULL)
    {
        strncpy(fileName, fileStart, fileNameLength);
        fileName[fileNameLength] = '\0';
    }
    return fileName;
}

// get file's extern name
char *getFileExtension(const char *filePath)
{
    const char *lastDot = strrchr(filePath, '.');
    if (lastDot == NULL || lastDot == filePath)
    {
        return NULL;
    }
    size_t extensionLength = strlen(lastDot) - 1;
    char *extension = (char *)malloc(extensionLength + 1);
    if (extension != NULL)
    {
        strcpy(extension, lastDot + 1);
    }
    return extension;
}

// uint8_t writeDirectory(FILE *file, const BootSector_t *bs, uint16_t clusterDirectory, DirectoryEntry_t *entryOut, DirectoryEntry_t *entryTemp)
// {
// }

error_code_t addContent(FILE *file, const BootSector_t *bs, const char *filePath, uint16_t startCluster, DirectoryEntry_t *entryOut)
{
    // check old file

    // get size of file
    uint32_t tempSize = getSizeFile(filePath);
    uint32_t addressCluster;

    // compare with size of cluster
    if (tempSize <= bs->bytesPerSector * bs->sectorsPerCluster)
    {
        addressCluster = getAddressCluster(bs, findTempEntryFat(file, bs));
        writeContent(file, bs, addressCluster, filePath, tempSize); // write content in data area
        return 0;                                                   /// OK
    }
    else
    {
    }
}