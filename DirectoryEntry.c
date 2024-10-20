#include "DirectoryEntry.h"
#include <stdio.h>
#include <stdint.h>


uint32_t getRootDirStart(const BootSector_t *bs){
    uint32_t rootDirSector = bs->reservedSectors + (bs->numberOfFATs * bs->sectorsPerFAT16);
    return rootDirSector * bs->bytesPerSector;
}

void read_root_directory(FILE *fp, const BootSector_t *bs){
    
    fseek(fp, getRootDirStart(bs), SEEK_SET);

    int i; 
    for(i =0; i < bs->rootEntryCount; i++){
        DirectoryEntry entry;
        if(fread(&entry, sizeof(DirectoryEntry), 1, fp) != 1){
            perror("Error reading directory entry");
            return;
        }

        if(entry.name[0] == 0x00){
            break;
        }
        
        if(entry.name[0] != 0xE5){
            printf("File Name: %.8s.%.3s\n", entry.name, entry.ext);
            printf("Size: %u bytes\n", entry.fileSize);
            printf("Starting Cluster: %u\n", entry.startCluster);
            printf("Attributes: 0x%X\n", entry.attr);
            printf("Date: %d/%d/%d\n", entry.date.year + 1980, entry.date.month, entry.date.day);
            printf("Time: %d:%d:%d\n", entry.time.hour, entry.time.min, entry.time.sec);
            printf("------------------------------\n");
        }
    }
}


