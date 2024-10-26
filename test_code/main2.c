#include "gui.h"

DirectoryEntry_t entry;

int main(){
    entry.name[0] = 't';
    entry.name[1] = 'e';
    entry.name[2] = 's';
    entry.name[3] = 't';
    entry.name[4] = '1';
    entry.name[5] = ' ';
    entry.name[6] = ' ';
    entry.name[7] = ' ';
    entry.ext[0] = 't';
    entry.ext[1] = 'x';
    entry.ext[2] = 't';
    entry.ext[3] = '\0';
    entry.attr = 0x20;
    entry.fileSize = 1024;
    entry.startCluster = 2;
    entry.date.year = 2021 - 1980;
    entry.date.month = 10;
    entry.date.day = 10;
    entry.time.hour = 10;
    entry.time.min = 10;
    entry.time.sec = 10;
    printDirectoryEntry(&entry);

}