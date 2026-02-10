// createlistofprimes.c
//
// Summary:
//   Generates a list of prime numbers based on a numeric limit provided
//   via the command line and saves it to an output file.
//
// Description:
//   This program computes all prime numbers up to a specified limit and
//   stores them in a file. It is intended to serve as a helper tool for
//   other applications that work with prime numbers.
//
//   The numeric limit must be greater than zero and smaller than the
//   maximum value supported by the Number type, defined in
//   commonfunctions.h.
//
// Usage:
//   createlistofprimes <limit> [--file-name]
//
// Options:
//   --file-name
//     Appends the limit value to the output file name.
//
// Output:
//   A file containing all prime numbers up to the specified limit is
//   generated.
//
//   The largest prime number found during execution is saved in a log
//   file for later use by other applications.
//
// Dependencies:
//   "commonfunctions.h" - Common utility functions and type definitions
//   <stdio.h>
//   <stdlib.h>
//
// Author:
//   Helder Lima


#include "commonfunctions.h"

#include <stdio.h>
#include <stdlib.h>


int main(int argc, char* argv[])
{
    Number   counter =     0;
    Number   number =      0;
    Number   testnumber =  0;
    Number*  primeslist =  NULL;
    Mode_t   mode =        NULL_MODE;
    Status_t status =      NULL_STATUS;
    char     filename[MAX_FILE_NAME_SIZE] = "";

    // Read the commands and save the entered number
    if (argc > 1)

        mode = readcommands(argv, &number);

    else
    {
        printf("\ncreatelistofprimes: error with argument format\n");

        return -1;
    }

    if (mode < 0)
    {
        printf("\ncreatelistofprimes: error with argument format\n");

        return -2;
    }

    // Determine the output file name format
    if (mode == NEWNUM)

        status = getfilename(filename, &number);

    else if (mode == NEWNONUM)

        status = getfilename(filename, NULL);
    
    else
    {
        printf("\ncreatelistofprimes: command error (%d)\n", mode);

        return -3;
    }

    if (status < 0)
    {
        printf("\ncreatelistofprimes: error with file name (%d)\n", status);

        return -4;
    }

    // Create an array for prime numbers
    primeslist = makeprimesarray(number);

    if (!primeslist)
    {
        printf("\ncreatelistofprimes: error "
               "allocating array of prime numbers\n");

        return -5;
    }

    // Test the primality of numbers
    while (testnumber <= number)
    {
        if (isprime(testnumber, primeslist))
            primeslist[counter++] = testnumber;

        testnumber++;
    }

    printf("\nCreating file with prime numbers...\n");

    // Save the list of prime numbers to the output file
    status = savelist(primeslist, counter, filename);

    if (status == SUCCESS)
    {
        // Show the output file path to the user
        status = showpath(filename);

        // Non-critical error
        if (status < 0)
            printf("\ncreatelistofprimes: error showing path (%d)\n", status);
    }
    else
    {
        printf("\ncreatelistofprimes: error creating file (%d)\n", status);

        free(primeslist);

        return -6;
    }

    // Save the largest prime number found to a log file
    if(counter > 0)

        status = saveprimeinlog(primeslist[counter - 1]);

    else
        status = saveprimeinlog(primeslist[0]);

    if (status < 0)
    {
        printf("\ncreatelistofprimes: error saving log (%d)\n", status);

        free(primeslist);

        return -7;
    }

    free(primeslist);

    return 0;
}