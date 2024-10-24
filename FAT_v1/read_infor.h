/*
 ******************************************************************************
 * @file		read_infor.h                                                   *
 * @author	Luu Ngoc Anh                                                       *
 * @date		00/00/0000                                                     *
 ******************************************************************************
 */

#ifndef __READ_INFOR_H__
#define __READ_INFOR_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdio.h>
#include <stdint.h>
#pragma pack(1)

  typedef struct
  {
    uint8_t jump[3];           // Nhảy qua code bootstrap
    char oem[8];               // OEM Identifier
    uint16_t bytesPerSector;   // Số byte trên mỗi sector
    uint8_t sectorsPerCluster; // Số sector trên mỗi cluster
    uint16_t reservedSectors;  // Số sector dự trữ (thường là 1)
    uint8_t numberOfFATs;      // Số lượng bảng FAT
    uint16_t rootEntryCount;   // Số mục trong thư mục gốc (FAT12/16)
    uint16_t totalSectors16;   // Tổng số sector (FAT12/16)
    uint8_t mediaDescriptor;   // Media type descriptor
    uint16_t sectorsPerFAT16;  // Số sector trên mỗi bảng FAT (FAT12/16)
    uint16_t sectorsPerTrack;  // Số sector trên mỗi track
    uint16_t numberOfHeads;    // Số đầu đọc của ổ đĩa
    uint32_t hiddenSectors;    // Số sector ẩn trước phân vùng này
    uint32_t totalSectors32;   // Tổng số sector (FAT32)
    uint32_t sectorsPerFAT32;  // Số sector trên mỗi bảng FAT (FAT32)
  } BootSector_t;

  void read_boot_sector(FILE *fp, BootSector_t *bs);
  void display_boot_sector_info(const BootSector_t *bs);

#ifdef __cplusplus
}
#endif

#endif

/********************************* END OF FILE ********************************/
/******************************************************************************/