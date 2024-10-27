#include "error_codes.h"

/*******************************************************************************
* Code
******************************************************************************/
const char* get_error_message(error_code_t code)
{
    switch (code)
    {
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
        case ERROR_WRONG_ATTRIBUTE:
            return "Wrong attribute";
        case ERROR_NO_DIRECTORY_CHANGE:
            return "No directory change";
        case ERROR_NO_MORE_PREV_DIR:
            return "No more previous directory";
        case ERROR_UNKNOWN:
            return "Unknown error";
        case ERROR_INVALID_CLUSTER:
            return "Invalid cluster";
        case ERROR_DELETE_FOLDER:
            return "Delete folder error";
        default:
            return "Unknown error";
    }
}