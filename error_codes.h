#ifndef ERROR_CODES_H
#define ERROR_CODES_H

typedef enum {
    ERROR_OK = 0,
    ERROR_FILE_NOT_FOUND,
    ERROR_READ_FAILURE,
    ERROR_WRITE_FAILURE,
    ERROR_MEMORY_ALLOCATION,
    ERROR_INVALID_PARAMETER,
    ERROR_INVALID_NAME,
    ERROR_UNKNOWN
} error_code_t;

const char* get_error_message(error_code_t code);

#endif // ERROR_CODES_H