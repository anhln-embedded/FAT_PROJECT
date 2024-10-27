#include "hal.h"

FILE* g_stream;

void HAL_intit(FILE *stream){
    g_stream = stream;
}

size_t HAL_fread(void* ptr, size_t size, size_t count)
{
    return fread(ptr, size, count, g_stream);
}

int HAL_fseek(long offset)
{
    return fseek(g_stream, offset, SEEK_SET);
}

char HAL_fgetc(void)
{
    return fgetc(g_stream);
}