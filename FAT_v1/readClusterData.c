#include "readClusterData.h"

// Hàm hiển thị ngày --- Quang Anh làm rồi
void displayDate(Date_t date)
{
    printf("%02d/%02d/%04d", date.day, date.month, date.year + 1980);
}

// Hàm hiển thị thời gian ---- Quang Anh làm rồi
void displayTime(Time_t time)
{
    printf("%02d:%02d:%02d", time.hour, time.min, time.sec * 2); // SEC * 2 vì đơn vị là 2 giây
}

// Hàm đọc và hiển thị nội dung của một folder
void readFolder(uint16_t startCluster, FILE *file)
{
    int i = 0;
    DirectoryEntry folder[ENTRIES_PER_CLUSTER]; // Giả định rằng ta đã nạp nội dung vào folder /// đưa dữ liệu vào folder[16] -- 16 là tổng số entries tối đa trong một cl
    // đưa data từ cluster vào folder
    fseek(file, (startCluster + 33 - 2) * 512, SEEK_SET);

    uint8_t buffer[CLUSTER_SIZE];
    fread(buffer, 1, CLUSTER_SIZE, file);
    /// đưa dữ liệu buffer sang dạng struct

    for (i = 0; i < ENTRIES_PER_CLUSTER; i++)
    {
        DirectoryEntry *entryTemp = (DirectoryEntry *)&buffer[i * DIR_ENTRY_SIZE];
        folder[i] = *entryTemp;
    }

    ////////////////////////////////

    printf("Reading folder starting at cluster: %d\n", startCluster);

    // Duyệt qua các entry trong thư mục

    for (i = 0; i < 16; i++)
    {
        DirectoryEntry entry = folder[i];

        // Nếu entry có tên rỗng, có nghĩa là hết danh sách các mục trong thư mục
        if (entry.name[0] == 0x00)
        {
            break;
        }

        // Kiểm tra nếu entry bị đánh dấu xóa
        if (entry.name[0] == 0xE5)
        {
            continue;
        }

        // Hiển thị tên tệp hoặc thư mục
        printf("Name: %.8s", entry.name);

        if (!(entry.attr & ATTR_DIRECTORY))
        {
            printf(".%.3s", entry.ext); // Hiển thị phần mở rộng nếu là tệp tin
        }

        printf("\n");

        // Hiển thị thuộc tính tệp hoặc thư mục
        if (entry.attr & ATTR_DIRECTORY)
        {
            printf("Type: Directory\n");
        }
        else
        {
            printf("Type: File\n");
            printf("Size: %d bytes\n", entry.fileSize);
        }

        // Hiển thị thời gian tạo
        printf("Created: ");
        displayDate(entry.date); //// Quang Anh Làm
        printf(" ");
        displayTime(entry.time); /// Quang Anh Làm
        printf("\n");

        // Hiển thị cluster bắt đầu của tệp/thư mục
        printf("Start Cluster: %d\n\n", entry.startCluster);
    }
}

void changeFolder(uint16_t startClusterCurentFolder, char *str)
{
    
}