// commonfunctions.c
//
// Summary:
//   Implements common utility functions and data types for programs
//   that generate and update prime number lists.
//
// Description:
//   This module provides shared functionality used by multiple
//   applications in the project related to prime number processing,
//   including list generation, validation, and auxiliary operations.
//
//   Its purpose is to centralize reusable logic and avoid code
//   duplication across executables.
//
// Interface:
//   This source file implements the interface declared in
//   commonfunctions.h.
//
// Assumptions:
//   All functions assume that input data has been previously validated
//   by the calling code. Error checking and argument verification are
//   the responsibility of the caller.
//
// Dependencies:
//   "commonfunctions.h"
//   <ctype.h>
//   <direct.h>   - Directory handling and _getcwd function
//   <math.h>
//   <stdio.h>
//   <stdlib.h>
//   <string.h>
//
// Author:
//   Helder Lima


#include "commonfunctions.h"

#include <ctype.h>
#include <direct.h>             // _getcwd
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// Parameters for functions
#define BASE        10          // Base used in strtoul
#define GETDIR      _getcwd     // Function to get the directory name
#define PNT_FACTOR  1.3         // Safety factor for the Prime Number Theorem


// Prototypes of auxiliary functions
int isinvalidnumber(const char*);
Status_t writeheaderline(FILE* const, Number, Number);
Status_t writenumber(FILE* const, Number);


// MAIN FUNCTIONS


Status_t getfilename(char* const filename, const Number* const number)
{
    char num[MAX_NUMBER_SIZE];
    Status_t status;

    // Test if there is an actual value or a null pointer
    if (number)
    {
        // Convert to string
        status = Numbertostr(num, number);

        if (status < 0)
            return WRITING_ERROR;

        // Construct the file name
        strcat(filename, FILE_NAME_LONG);

        strcat(filename, num);

        strcat(filename, FILE_EXTENSION);
    }
    else
    {
        strcat(filename, FILE_NAME_SHORT);

        strcat(filename, FILE_EXTENSION);
    }

    return SUCCESS;
}


int isprime(Number number, Number* primes)
{
    int answer = 1;
    Number limit;

    // Test natural numbers up to 5
    if (number <= 5)
        return (number < 2 || number == 4) ? 0 : 1;

    // Limit for testing
    limit = (Number)ceil(sqrt(number));

    // Test the other natural numbers
    while (*primes <= limit)
    {
        if (number % *primes == 0)
        {
            answer = 0;

            break;
        }
        else
            primes++;               // Next prime
    }

    return answer;
}


Number* makeprimesarray(Number limit)
{
    Number* array = NULL;
    Number listsize = 0;

    if (limit > 1)
    {
        // According to the Prime Number Theorem
        listsize = (Number)ceil(PNT_FACTOR * (limit / log(limit)));
    }
    else
        listsize = 1;    

    // Array automatically initialized with zeros
    array = (Number*)calloc(listsize, sizeof(Number));

    return (array == NULL) ? NULL : array;
}


Status_t Numbertostr(char* const str, const Number* const number)
{
    int status = 0;

    if (!number)
        return NULL_POINTER_ERROR;

    // Convert Number to a string
    status = snprintf(str, MAX_NUMBER_SIZE, "%lu", *number);

    if (status >= 0 && status < MAX_NUMBER_SIZE)

        return SUCCESS;

    else
        return WRITING_ERROR;
}


Mode_t readcommands(const char* const* argv, Number* number)
{
    int error = 0;
    int isnumber = 0;
    int name = 0;
    int numsaved = 0;
    int update = 0;

    while (*++argv)
    {
        isnumber = isdigit(**argv);

        // Test the consistency and types of arguments
        if (isnumber && numsaved)
        {
            error = 1;

            break;
        }
        else if (isnumber)
        {
            if (isinvalidnumber(*argv))
            {
                error = 1;

                break;
            }

            // Convert to Number
            *number = strtoNumber(*argv);

            // Limit violation
            if (*number == 0)
            {
                error = 1;

                break;
            }

            numsaved = 1;
        }
        else
        {
            // Deal with flags
            if (name == 0 && strcmp(*argv, "--file-name") == 0)

                name = 1;

            else if (update == 0 && strcmp(*argv, "--update") == 0)

                update = 1;

            else
            {
                error = 1;

                break;
            }
        }
    }

    if (!numsaved)
        error = 1;

    // Determine the execution mode
    if (error)

        return ERROR_MODE;

    else if (update && name)

        return UPDTNUM;

    else if (update)

        return UPDTNONUM;

    else if (name)

        return NEWNUM;

    else
        return NEWNONUM;
}


Status_t savelist(const Number* list, Number quantity, char* filename)
{
    int check = 0;
    Status_t status;
    FILE* file = NULL;

    file = fopen(filename, "w");

    if (!file)
        return FILE_ERROR;

    // Write the first line of the file
    if(quantity > 0)

        status = writeheaderline(file, quantity, list[quantity - 1]);

    else
        status = writeheaderline(file, quantity, list[0]);

    if (status < 0)
    {
        fclose(file);

        return WRITING_ERROR;
    }

    // Write the prime numbers
    while (*list)
    {
        check = fprintf(file, "\n");

        status = writenumber(file, *list++);

        if (check < 0 || status < 0)
        {
            fclose(file);

            return WRITING_ERROR;
        }
    }

    fclose(file);

    return SUCCESS;
}


Status_t saveprimeinlog(Number prime)
{
    Status_t status;
    FILE* file = NULL;

    // Create the log
    file = fopen(PRIME_LOG_FILE_NAME, "w");

    if (!file)
        return FILE_ERROR;

    // Write the number in the log
    status = writenumber(file, prime);

    if (status < 0)
    {
        fclose(file);
        
        return WRITING_ERROR;
    }

    fclose(file);

    return SUCCESS;
}


Status_t showpath(const char* const filename)
{
    char fullname[MAX_FILE_NAME_SIZE];
    char* dirname = NULL;
    char* file = NULL;
    char* folder = NULL;

    // Copy the full file name
    strcpy(fullname, filename);

    // Get the folder name
    folder = strtok(fullname, "\\");

    // Get the file name
    file = strtok(NULL, "\\");

    if (!folder || !file)
        return FORMAT_ERROR;
    
    // Get the name of the current working directory
    dirname = GETDIR(NULL, _MAX_DIR);

    if (!dirname)
        return GETCWD_ERROR;

    // Display the path
    printf("\nThe list of prime numbers was saved in the file \'%s\'...\n"
           "\n... in the directory:\n\n%s\\%s\n", file, dirname, folder);

    free(dirname);

    return SUCCESS;
}


Number strtoNumber(const char* const str)
{
    Number number;

    // Convert to unsigned long (Number)
    number = strtoul(str, NULL, BASE);

    // Return 'number' if there is no limit violation
    return (number == ULONG_MAX) ? 0 : number;
}


// AUXILIARY FUNCTIONS


// Test if the number format is incorrect.
//
int isinvalidnumber(const char* str)
{
    while (*str != '\0')
    {
        if (!isdigit(*str++))
            return 1;
    }

    return 0;
}


// Write the intro line of the prime numbers file.
// Return the corresponding status: success, error.
//
Status_t writeheaderline(FILE* const stream,
                         Number quantity,
                         Number lastnumber)
{
    char quant[MAX_NUMBER_SIZE];
    char lastn[MAX_NUMBER_SIZE];
    int check = 0;
    Status_t status;

    // Convert numbers to string
    status = Numbertostr(quant, &quantity);

    if (status < 0)
        return WRITING_ERROR;

    status = Numbertostr(lastn, &lastnumber);

    if (status < 0)
        return WRITING_ERROR;

    // Write the string in the file
    check = fprintf(stream, "%s%s%s%s",
                    HEADER_LINE_PT1, quant, HEADER_LINE_PT2, lastn);

    return (check < 0) ? WRITING_ERROR : SUCCESS;
}


// Write a number to the prime number file.
// Return the corresponding status: success, error.
//
Status_t writenumber(FILE* const stream, Number number)
{
    char num[MAX_NUMBER_SIZE];
    int check = 0;
    Status_t status;

    // Convert number to string
    status = Numbertostr(num, &number);

    if (status < 0)
        return WRITING_ERROR;

    // Write the number in the file
    check = fprintf(stream, "%s", num);

    return (check < 0) ? WRITING_ERROR : SUCCESS;
}
