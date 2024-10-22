#ifndef  _READ_CLUSTER_DATA_
#define _READ_CLUSTER_DATA_

#include    <stdint.h>
#include    <stdio.h>
#include    <string.h>

// Cấu trúc ngày tháng năm
typedef struct
{
    uint16_t day : 5;
    uint16_t month : 4;
    uint16_t year : 7;
} Date_t;
/*Lưu ý rằng year bắt đầu từ 1980*/

// Cấu trúc thời gian
typedef struct
{
    uint16_t hour: 5;
    uint16_t minute : 6;
    uint16_t sec : 5;
} Time_t;

// Cấu trúc entry trong thư mục
typedef struct {
    char name[8];
    char ext[3];
    uint8_t attr;
    uint8_t reserved[10];
    Time_t time;
    Date_t date;
    uint16_t startCluster;
    uint32_t fileSize;
} DirectoryEntry;


// Hàm hiển thị ngày --- Quang Anh làm rồi
void displayDate(Date_t date) {
    printf("%02d/%02d/%04d", date.day, date.month, date.year + 1980);
}

// Hàm hiển thị thời gian ---- Quang Anh làm rồi
void displayTime(Time_t time) {
    printf("%02d:%02d:%02d", time.hour, time.minute, time.sec * 2);  // SEC * 2 vì đơn vị là 2 giây
}


#define ATTR_DIRECTORY  0x10  // folder
#define ATTR_FILE       0x20  // file

#define CLUSTER_SIZE 512
#define DIR_ENTRY_SIZE 32
#define ENTRIES_PER_CLUSTER (CLUSTER_SIZE / DIR_ENTRY_SIZE)



#endif
