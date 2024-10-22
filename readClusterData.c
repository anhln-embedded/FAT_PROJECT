#include    "readClusterData.h"

// Hàm đọc và hiển thị nội dung của một folder
void readFolder(uint16_t startCluster, FILE *fp)
{
    DirectoryEntry folder[ENTRIES_PER_CLUSTER];  // Giả định rằng ta đã nạp nội dung vào folder /// đưa dữ liệu vào folder[16] -- 16 là tổng số entries tối đa trong một cl
    // đưa data từ cluster vào folder
    fseek(fp, startCluster, SEEK_SET);

    uint8_t buffer[CLUSTER_SIZE];
    fread(buffer, 1, CLUSTER_SIZE, fp);
    /// đưa dữ liệu buffer sang dạng struct

    for (uint8_t i=0; i< ENTRIES_PER_CLUSTER; i++)
    {
        DirectoryEntry* entryTemp = (DirectoryEntry*)&buffer[i * DIR_ENTRY_SIZE];
        folder[i] = *entryTemp;
    }

////////////////////////////////

    printf("Reading folder starting at cluster: %d\n", startCluster);

    // Duyệt qua các entry trong thư mục
    for (int i = 0; i < 16; i++) {
        DirectoryEntry entry = folder[i];

        // Nếu entry có tên rỗng, có nghĩa là hết danh sách các mục trong thư mục
        if (entry.name[0] == 0x00) {
            break;
        }

        // Kiểm tra nếu entry bị đánh dấu xóa
        if (entry.name[0] == 0xE5) {
            continue;
        }

        // Hiển thị tên tệp hoặc thư mục
        printf("Name: %.8s", entry.name);

        if (!(entry.attr & ATTR_DIRECTORY)) {
            printf(".%.3s", entry.ext);  // Hiển thị phần mở rộng nếu là tệp tin
        }

        printf("\n");

        // Hiển thị thuộc tính tệp hoặc thư mục
        if (entry.attr & ATTR_DIRECTORY) {
            printf("Type: Directory\n");
        } else {
            printf("Type: File\n");
            printf("Size: %d bytes\n", entry.fileSize);
        }

        // Hiển thị thời gian tạo
        printf("Created: ");
        displayDate(entry.date);  //// Quang Anh Làm
        printf(" ");
        displayTime(entry.time); /// Quang Anh Làm
        printf("\n");

        // Hiển thị cluster bắt đầu của tệp/thư mục
        printf("Start Cluster: %d\n\n", entry.startCluster);
    }
}
