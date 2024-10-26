#include "gui.h"
#include <stdio.h>
#include <string.h>

void printCentered(const char *str, int width)
{
    int len = strlen(str);
    int padding = (width - len) / 2;
    printf("%*s%s%*s", padding, "", str, padding + (width - len) % 2, "");
}

void printDirectoryEntry(const DirectoryEntry_t *entry)
{
    char fileName[13] = {0};
    strncpy(fileName, entry->name, 8);
    fileName[8] = '\0';

    int i;
    for (i = strlen(fileName) - 1; i >= 0 && fileName[i] == ' '; i--)
    {
        fileName[i] = '\0';
    }

    if ((entry->attr & ATTR_DIRECTORY) == 0 && entry->ext[0] != ' ')
    {
        strncat(fileName, ".", 1);
        strncat(fileName, entry->ext, 3);
    }

    for (i = strlen(fileName) - 1; i >= 0 && fileName[i] == ' '; i--)
    {
        fileName[i] = '\0';
    }

    // Accessing the time and date components
    unsigned char hours = entry->time.hour;
    unsigned char minutes = entry->time.min;
    unsigned char seconds = entry->time.sec * 2; // Assuming seconds are stored in 2-second increments
    unsigned short year = entry->date.year + 1980;
    unsigned char month = entry->date.month;
    unsigned char day = entry->date.day;


    // Calculate column widths
    int nameWidth = strlen(fileName) > strlen("Name") ? strlen(fileName) : strlen("Name");
    int attrWidth = strlen("Attribute");
    int timeWidth = strlen("10:30:30") > strlen("Time") ? strlen("10:30:30") : strlen("Time");
    int dateWidth = strlen("15/08/2001") > strlen("Date") ? strlen("15/08/2001") : strlen("Date");
    int startClusterWidth = strlen("2") > strlen("Start Cluster") ? strlen("2") : strlen("Start Cluster");
    int fileSizeWidth = strlen("1024") > strlen("File Size") ? strlen("1024") : strlen("File Size");

    // Print the table header
    printf("+-%-*s-+-%-*s-+-%-*s-+-%-*s-+-%-*s-+-%-*s-+\n",
           nameWidth, "----------",
           attrWidth, "-----------",
           timeWidth, "----------",
           dateWidth, "------------",
           startClusterWidth, "---------------",
           fileSizeWidth, "-----------");
    printf("| ");
    printCentered("Name", nameWidth);
    printf(" | ");
    printCentered("Attribute", attrWidth);
    printf(" | ");
    printCentered("Time", timeWidth);
    printf(" | ");
    printCentered("Date", dateWidth);
    printf(" | ");
    printCentered("Start Cluster", startClusterWidth);
    printf(" | ");
    printCentered("File Size", fileSizeWidth);
    printf(" |\n");
    printf("+-%-*s-+-%-*s-+-%-*s-+-%-*s-+-%-*s-+-%-*s-+\n",
           nameWidth, "----------",
           attrWidth, "-----------",
           timeWidth, "----------",
           dateWidth, "------------",
           startClusterWidth, "----------------",
           fileSizeWidth, "-----------");

    // Print the values
    printf("| ");
    printCentered(fileName, nameWidth);
    printf(" | ");
    char attrStr[12];
    snprintf(attrStr, sizeof(attrStr), "%u", entry->attr);
    printCentered(attrStr, attrWidth);
    printf(" | ");
    char timeStr[9];
    snprintf(timeStr, sizeof(timeStr), "%02d:%02d:%02d", hours, minutes, seconds);
    printCentered(timeStr, timeWidth);
    printf(" | ");
    char dateStr[11];
    snprintf(dateStr, sizeof(dateStr), "%02d/%02d/%04d", day, month, year);
    printCentered(dateStr, dateWidth);
    printf(" | ");
    char startClusterStr[14];
    snprintf(startClusterStr, sizeof(startClusterStr), "%u", entry->startCluster);
    printCentered(startClusterStr, startClusterWidth);
    printf(" | ");
    char fileSizeStr[10];
    snprintf(fileSizeStr, sizeof(fileSizeStr), "%u", entry->fileSize);
    printCentered(fileSizeStr, fileSizeWidth);
    printf(" |\n");

    // Print the table footer
    printf("+-%-*s-+-%-*s-+-%-*s-+-%-*s-+-%-*s-+-%-*s-+\n",
           nameWidth, "----------",
           attrWidth, "-----------",
           timeWidth, "----------",
           dateWidth, "------------",
           startClusterWidth, "----------------",
           fileSizeWidth, "-----------");
}

// void printDirectoryEntry(const DirectoryEntry_t *entry){
//     printf("File Name: %.8s.%.3s\n", entry->name, entry->ext);
//     printf("Size: %u bytes\n", entry->fileSize);
//     printf("Starting Cluster: %u\n", entry->startCluster);
//     printf("Attributes: 0x%X\n", entry->attr);
//     printf("Date: %d/%d/%d\n", entry->date.year + 1980, entry->date.month, entry->date.day);
//     printf("Time: %d:%d:%d\n", entry->time.hour, entry->time.min, entry->time.sec);
//     printf("------------------------------\n");
// }