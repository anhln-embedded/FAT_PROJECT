#ifndef _HAL_H_
#define _HAL_H_

#include <stdio.h>

void HAL_intit(FILE *stream);
size_t HAL_fread(void *ptr, size_t size, size_t count);
int HAL_fseek(long offset);
char HAL_fgetc(void);

#endif