#include "gui.h"
#include <stdio.h>
#include <string.h>

/*******************************************************************************
* Variables
******************************************************************************/
static unsigned char hours;
static unsigned char minutes;
static unsigned char seconds; /* Assuming seconds are stored in 2-second increments */
static unsigned short year;
static unsigned char month;
static unsigned char day;

/* Calculate column widths */
static int nameWidth = 35;
static int typeWidth = 6; /* New column for type (File/Folder) */
static int timeWidth = 11;
static int dateWidth = 11;
/* static int startClusterWidth = 11; */
static int fileSizeWidth = 11;

/*******************************************************************************
* Code
******************************************************************************/
void printCentered(const char *str, int width)
{
    int len = strlen(str);
    int padding = (width - len) / 2;
    printf("%*s%s%*s", padding, "", str, padding + (width - len) % 2, "");
}

void printHeader(void)
{
    /* Print the table header */
    printf("+-%-*s-+-%-*s-+-%-*s-+-%-*s-+-%-*s-+\n",
           nameWidth, "-----------------------------------",
           typeWidth, "------",
           timeWidth, "-----------",
           dateWidth, "-----------",
           /* startClusterWidth, "---------------", */
           fileSizeWidth, "-----------");
    printf("| ");
    printCentered("Name", nameWidth);
    printf(" | ");
    printCentered("Type", typeWidth);
    printf(" | ");
    printCentered("Time", timeWidth);
    printf(" | ");
    printCentered("Date", dateWidth);
    printf(" | ");
    /* printCentered("Start Cluster", startClusterWidth); */
    /* printf(" | "); */
    printCentered("File Size", fileSizeWidth);
    printf(" |\n");

    printf("+-%-*s-+-%-*s-+-%-*s-+-%-*s-+-%-*s-+\n",
           nameWidth, "-----------------------------------",
           typeWidth, "------",
           timeWidth, "-----------",
           dateWidth, "-----------",
           /* startClusterWidth, "----------------", */
           fileSizeWidth, "-----------");
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

    hours = entry->time.hour;
    minutes = entry->time.min;
    seconds = entry->time.sec * 2; /* Assuming seconds are stored in 2-second increments */
    year = entry->date.year + 1980;
    month = entry->date.month;
    day = entry->date.day;

    /* Print the values */
    printf("| ");
    printCentered(fileName, nameWidth);
    printf(" | ");
    char typeStr[7];
    snprintf(typeStr, sizeof(typeStr), "%s", (entry->attr & ATTR_DIRECTORY) ? "Folder" : "File");
    printCentered(typeStr, typeWidth);
    printf(" | ");
    char timeStr[9];
    snprintf(timeStr, sizeof(timeStr), "%02d:%02d:%02d", hours, minutes, seconds);
    printCentered(timeStr, timeWidth);
    printf(" | ");
    char dateStr[11];
    snprintf(dateStr, sizeof(dateStr), "%02d/%02d/%04d", day, month, year);
    printCentered(dateStr, dateWidth);
    printf(" | ");
    /* char startClusterStr[14]; */
    /* snprintf(startClusterStr, sizeof(startClusterStr), "%u", entry->startCluster); */
    /* printCentered(startClusterStr, startClusterWidth); */
    /* printf(" | "); */
    char fileSizeStr[10];
    snprintf(fileSizeStr, sizeof(fileSizeStr), "%u", entry->fileSize);
    printCentered(fileSizeStr, fileSizeWidth);
    printf(" |\n");

    /* Print the table footer */
    printf("+-%-*s-+-%-*s-+-%-*s-+-%-*s-+-%-*s-+\n",
           nameWidth, "-----------------------------------",
           typeWidth, "------",
           timeWidth, "-----------",
           dateWidth, "-----------",
           /* startClusterWidth, "----------------", */
           fileSizeWidth, "-----------");
}

void printHelp(void)
{
    printf("+------------------+---------------------------------------------+\n");
    printf("|     Command      |                 Description                 |\n");
    printf("+------------------+---------------------------------------------+\n");
    printf("| ls               | List files in the current directory         |\n");
    printf("| ls -a            | List all files, including hidden files      |\n");
    printf("| cd <dir>         | Change the current directory to <dir>       |\n");
    printf("| cat <file>       | Display the contents of <file>              |\n");
    printf("| exit             | Exit the terminal or command line interface |\n");
    printf("+------------------+---------------------------------------------+\n");
}