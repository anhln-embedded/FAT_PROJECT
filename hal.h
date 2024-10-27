#ifndef _HAL_H_
#define _HAL_H_

#include <stdio.h>
#include <stdint.h>

/*******************************************************************************
* API
******************************************************************************/

/**
 * @brief Initialize the HAL with a given stream.
 * 
 * @param stream Pointer to the file stream.
 */
void HAL_init(FILE *stream);

/**
 * @brief Read data from the HAL.
 * 
 * @param ptr Pointer to the buffer where the read data will be stored.
 * @param size Size of each element to be read.
 * @param count Number of elements to be read.
 * @return Number of elements successfully read.
 */
size_t HAL_fread(void *ptr, size_t size, size_t count);

/**
 * @brief Set the position indicator associated with the stream.
 * 
 * @param offset Offset value to set the position indicator.
 * @return 0 on success, non-zero on failure.
 */
int HAL_fseek(uint32_t offset);

/**
 * @brief Get a character from the HAL.
 * 
 * @return The character read as an unsigned char cast to an int or EOF on end of file or error.
 */
char HAL_fgetc(void);

/**
 * @brief Writes data to a file.
 *
 * This function writes `count` elements of data, each `size` bytes long, 
 * from the buffer pointed to by `ptr` to the file.
 *
 * @param ptr Pointer to the buffer containing the data to be written.
 * @param size Size, in bytes, of each element to be written.
 * @param count Number of elements, each of size `size`, to be written.
 * @return The total number of elements successfully written, which may be less than `count` 
 *         if an error occurs or the end of the file is reached.
 */
size_t HAL_fwrite(const void *ptr, size_t size, size_t count);

#endif /* _HAL_H_ */