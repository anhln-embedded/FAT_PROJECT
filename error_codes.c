#include "error_codes.h"

const char* get_error_message(error_code_t code) {
    switch (code) {
        case ERROR_OK:
            return "No error";
        case ERROR_FILE_NOT_FOUND:
            return "File not found";
        case ERROR_READ_FAILURE:
            return "Read failure";
        case ERROR_WRITE_FAILURE:
            return "Write failure";
        case ERROR_MEMORY_ALLOCATION:
            return "Memory allocation failure";
        case ERROR_INVALID_PARAMETER:
            return "Invalid parameter";
        case ERROR_INVALID_NAME:
            return "Invalid name";    
        case ERROR_UNKNOWN:
        default:
            return "Unknown error";
    }
}