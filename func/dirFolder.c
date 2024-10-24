#include "dirFolder.h"

void dirFolder(uint16_t addressEntryFAT, FILE * file)
{
    uint16_t tempAddFAT = readEntryFAT(addressEntryFAT, file);
    uint16_t tempAddCluster;
    do
    {
        if ( tempAddFAT == STOP_READ_ENTRY)
        {
            return 1; // out loop
        }
        else
        {
            tempAddCluster = fat2cluster(addressEntryFAT, file);
            readFolder(tempAddCluster, file);
        }
        tempAddFAT = readEntryFAT(tempAddFAT, file);
    } while (1);
    
}
