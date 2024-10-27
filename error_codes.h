#ifndef ERROR_CODES_H
#define ERROR_CODES_H

/*******************************************************************************
* Definitions
******************************************************************************/

/**
 * @brief Error codes used throughout the project.
 */
typedef enum {
    ERROR_OK = 0,
    ERROR_FILE_NOT_FOUND,
    ERROR_READ_FAILURE,
    ERROR_WRITE_FAILURE,
    ERROR_MEMORY_ALLOCATION,
    ERROR_INVALID_PARAMETER,
    ERROR_INVALID_NAME,
    ERROR_UNKNOWN,
    ERROR_WRONG_ATTRIBUTE,
    ERROR_NO_DIRECTORY_CHANGE,
    ERROR_NO_MORE_PREV_DIR,
} error_code_t;

/*******************************************************************************
* API
******************************************************************************/

/**
 * @brief Get the error message corresponding to the error code.
 * 
 * @param code The error code.
 * @return The error message as a string.
 */
const char* get_error_message(error_code_t code);

#endif /* ERROR_CODES_H */