// commonfunctions.h
//
// Summary:
//   Declares common utility functions, data types, and macros for prime
//   number processing applications.
//
// Description:
//   This header provides the public interface for shared functionality
//   used across multiple programs related to prime number list
//   generation and updating.
//
// Provided Interface:
//   - Prime generation functions
//   - Validation utilities
//   - Number type definition
//   - Project-specific macros
//
// Usage:
//   #include "commonfunctions.h"
//
// Assumptions:
//   All inputs must be validated before calling the functions.
//
// Dependencies:
//   None
//
// Author:
//   Helder Lima


#ifndef COMMONFUNCTIONS_H
#define COMMONFUNCTIONS_H


// Type used to represent prime numbers
typedef unsigned long Number;


// Parameters used in files
#define FILE_EXTENSION       ".txt"
#define FILE_NAME_LONG       "lists\\list_of_primes_up_to_"  // File names
#define FILE_NAME_SHORT      "lists\\list_of_primes"         //
#define LOG_FILE_NAME        "logs\\log"                     //
#define PRIME_LOG_FILE_NAME  "logs\\primelog"                //
#define HEADER_LINE_PT1      "quantity="
#define HEADER_LINE_PT2      ",last="
#define MAX_FILE_LINE_SIZE   200
#define MAX_FILE_NAME_SIZE   115


// Maximum number of digits of type Number
#define MAX_NUMBER_SIZE  81


// Modes
typedef enum { NEWNONUM =   0,      // Create a file / name without number
               NEWNUM =     1,      // Create a file / name with number
               UPDTNONUM =  2,      // Update a file / name without number
               UPDTNUM =    3,      // Update a file / name with number

               ERROR_MODE = -1,
               NULL_MODE =  -1000
             } Mode_t;


// Status codes
typedef enum { SUCCESS =             0,
               FORMAT_ERROR =       -1,
               NULL_POINTER_ERROR = -2,
               FILE_ERROR =         -3,
               WRITING_ERROR =      -4,
               GETCWD_ERROR =       -5,
               NO_UPDATE_STATUS =   -100,
               NULL_STATUS =        -1000
             } Status_t;


// FUNCTIONS


// Construct the filename used for output.
// If number is a null pointer, the function constructs a short name without
// a reference number.
// Return the status corresponding to success or error.
//
Status_t getfilename(char* const filename, const Number* const number);


// Check if a natural number is prime.
// A list of prime numbers is required, the last value of which is greater
// than the square root of the number being tested, rounded up.
// For values less than or equal to 5, a list of prime numbers less than the
// value being tested is sufficient.
// Return 1 if the number is prime, and 0 otherwise.
//
int isprime(Number number, Number* primes);


// Allocate enough space to hold all prime numbers up to the limit value.
// The array is automatically initialized with zeros.
// Return NULL if there is an allocation error.
//
Number* makeprimesarray(Number limit);


// Convert a Number to a string.
// Return the corresponding status: success, error, or missing
// value (null case).
//
Status_t Numbertostr(char* const str, const Number* const number);


// Read the commands given by the command line and inform the program about
// the execution mode or if there was an error.
// The number entered on the command line is saved in 'number'.
//
Mode_t readcommands(const char* const* argv, Number* number);


// Save the list of prime numbers.
// The last value in the list of numbers must be zero to indicate its end.
// Return the status corresponding to success or error.
//
Status_t savelist(const Number* list, Number quantity, char* filename);


// Save a prime number to a log file.
// Return the status corresponding to success or error.
//
Status_t saveprimeinlog(Number prime);


// Display the directory path where the list of prime numbers was saved.
// Return the status corresponding to success or error.
//
Status_t showpath(const char* const filename);


// Convert a numeric string to Number format.
// If there is a conversion error or the value exceeds the maximum allowed,
// return zero.
// A string representing the value zero is correctly converted to zero.
//
Number strtoNumber(const char* const str);


#endif // !COMMONFUNCTIONS.H
