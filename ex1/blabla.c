//
// Created by Ishay Hil on 03/11/2019.
//

#include <stdio.h>
#include <stdlib.h> // Imported for "EXIT_FAILURE" and "EXIT_SUCCESS"
#include <string.h> // Imported for "strtok"

/**
 * @brief Defines the maximum line length.
 */
#define LINE_MAX_SIZE 1024

/**
 * @brief The delimiters used to separate the input
 */
#define INPUT_DELIMS " \t\r\n"

/**
 * @brief The main function that runs the program.
 * @param argc Non-negative value representing the number of arguments passed
 * to the program from the environment in which the program is run.
 * @param argv Pointer to the first element of an array of pointers to null-terminated
 * multibyte strings that represent the arguments passed to the program from the execution
 * environment. The value of argv[argc] is guaranteed to be a null pointer.
 * @return 0, to tell the system the execution ended without errors, or 1, to
 * tell the system that the code has execution errors.
 */
int main(int argc, const char* argv[])
{
    /* Initializing the variables that'll contain the line information */
    char line[LINE_MAX_SIZE];
    char *value = NULL;

    /* Reads the line from the stdin */
    if (fgets(line, LINE_MAX_SIZE, stdin) == NULL)
    {
        fprintf(stderr, "No input\n");
        return EXIT_FAILURE;
    }

    /* Now "line" contains the contents of the entire line, including the suffix "\n", and "strlen(line)" contains the line length.
     Lets read the first value. If there's no input, we'll get "NULL" */
    value = strtok(line, INPUT_DELIMS);

    /* Handle every value and read the next value */
    while (value != NULL)
    {
        /* Read the value and print it */
        size_t length = strlen(value);
        printf("Input String: %s (length: %lu)\n", value, length);

        /* Using the same approach as we've done in Java,
         * we can iterate over the string characters using standard
         * array script operations.
         * For example:
           <code>
            for (int i = 0; i < length; ++i) {
                // str[i] is the i'th character in the string.
                printf("%c", str[i]);
            }
           </code>
         */

        /* Read the next value */
        value = strtok(NULL, INPUT_DELIMS);
    }

    return EXIT_SUCCESS;
}