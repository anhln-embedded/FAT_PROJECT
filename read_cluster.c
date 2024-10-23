#include "read_cluster.h"
#include "DirectoryEntry.h"

uint32_t getNextCluster(FILE *fp, const BootSector_t *bs, uint32_t cluster)
{
    uint32_t NextCluster;
    uint32_t FATOffset = cluster * 2;
    uint32_t FATSector = bs->reservedSectors * bs->bytesPerSector + (FATOffset / bs->bytesPerSector);
    fseek(fp, FATSector, SEEK_SET);
    fread(&NextCluster, 4, 1, fp);
    NextCluster = NextCluster & 0x0FFFFFFF;
    return NextCluster;
}

void read_cluster(FILE *fp, const BootSector_t *bs, uint32_t cluster, char *buffer)
{
    uint32_t clusterSize = bs->bytesPerSector * bs->sectorsPerCluster;
    uint32_t clusterStart = bs->reservedSectors * bs->bytesPerSector + bs->numberOfFATs * bs->sectorsPerFAT16 * bs->bytesPerSector + bs->rootEntryCount * sizeof(DirectoryEntry);
    clusterStart += (cluster - 2) * clusterSize;
    fseek(fp, clusterStart, SEEK_SET);
    fread(buffer, clusterSize, 1, fp);
}

char * getFileContent(FILE *fp, const BootSector_t *bs, const DirectoryEntry *entry){
    uint32_t position = ftell(fp);
    // uint32_t sizeBuffer = entry->fileSize;
    // char *buffer = (char *)malloc(sizeBuffer + 1);
    // uint32_t clusterSize = bs->bytesPerSector * bs->sectorsPerCluster;
    // uint32_t cluster = entry->startCluster;
    // uint16_t cnt = 0;
    // while(sizeBuffer % clusterSize != 0){
    //     read_cluster(fp, bs, cluster, &buffer[cnt]);
    //     cluster = getNextCluster(fp, bs, cluster);
    //     cnt += clusterSize;
    //     sizeBuffer /= clusterSize;
    // }

    // if(sizeBuffer != 0){
    //     read_cluster(fp, bs, cluster, &buffer[cnt]);
    // }

    // buffer[entry->fileSize] = '\0';
    // printf("%s\n", buffer);
    // free(buffer);
    
    char *buffer = (char *)malloc(entry->fileSize + 1);
    uint32_t clusterSize = bs->bytesPerSector * bs->sectorsPerCluster; // 512
    uint32_t clusterAddress = bs->reservedSectors * bs->bytesPerSector + bs->numberOfFATs * bs->sectorsPerFAT16 * bs->bytesPerSector + bs->rootEntryCount * sizeof(DirectoryEntry);
    clusterAddress += (entry->startCluster - 2) * clusterSize;
    fseek(fp, clusterAddress, SEEK_SET);
    fread(buffer, entry->fileSize, 1, fp);
    buffer[entry->fileSize] = '\0';
    printf("%s\n", buffer);
    free(buffer);
    fseek(fp, position, SEEK_SET);
    return buffer;
}

// DirectoryEntry *getDirectoryEntry(FILE *fp, const BootSector_t *bs, const DirectoryEntry *entry){

// }