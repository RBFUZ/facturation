/**
 * Copyright 2010 Sébastien Aupetit <sebastien.aupetit@univ-tours.fr>
 *
 * This source code is free software: you can redistribute it and/or modify it under
 * the terms of the GNU Lesser General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option) any
 * later version.
 *
 * This source code is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more
 * details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this source code. If not, see <http://www.gnu.org/licenses/>.
 *
 * $Id$
 */

#ifndef FACTURATION_BASE_CONFIG_H
#define FACTURATION_BASE_CONFIG_H

/**
 * @defgroup Customer Customers related stuff
 * @defgroup Catalog Catalog related stuff
 */



#include <stdlib.h>
#include <stdio.h>
#include <math.h>


/* Allow the inclusion of GTK+ by ignoring an annoying warning due to GTK+ fuzziness */
#pragma GCC diagnostic ignored "-Wsign-conversion"
#pragma GCC diagnostic ignored "-Wstrict-prototypes"
#include <gtk/gtk.h>
#pragma GCC diagnostic error "-Wsign-conversion"
#pragma GCC diagnostic error "-Wstrict-prototypes"

#ifdef UNUSED
#elif defined(__GNUC__)
# define UNUSED(x) UNUSED_ ## x __attribute__((unused))
#elif defined(__LCLINT__)
# define UNUSED(x) /*@unused@*/ x
#else
/** Mark a parameter name as being unused to suppress compiler warnings.
 * @param name the name of the parameter.
 *
 * @remarks You only need to surround the parameter name with the macro.
 * @code
 * void functionName( int usedParam, double UNUSED(unusedParam) ) {
 * ...
 * }
 * @endcode
 */
# define UNUSED(name) name
#endif


/** Function which displays a message and halt the debugger before terminating the program
 * @param message the message to display
 * @param line the line from which the function was called
 * @param file the file from which the function was called
 */
void debugFatalError(const char * message, long line, const char * file) __attribute__ ((noreturn));

/**
 * Function which displays a message before terminating the program
 * @param message the message to display
 * @note While debugging, it simplifies the debugging by halting the debugger
 * before the application is terminated when the fatalerror() function is called.
 */
#define fatalError(message) debugFatalError(message, __LINE__, __FILE__)

/** Last name of the student. */
extern const char * STUDENT_LASTNAME;
/** First name of the student. */
extern const char * STUDENT_FIRSTNAME;
/** The current school years */
extern const char * STUDENT_YEARS;
/** The group of the students */
extern const char * STUDENT_GROUP;

/**
 * Compute the maximal value of the two parameters
 * @param x the first parameter
 * @param y the second parameter
 */
#define MAXVALUE(x, y) (((x)>(y))?(x):(y))

#define OVERRIDABLE_PREFIX extern
#define OVERRIDABLE(functionname) (*functionname)
#define IMPLEMENT(functionname) user_ ## functionname

/**
 * Setup the machinery needed for overridable functions.
 */
void setupOverridable(void);

/* Include these two prototypes since string.h can not be included without errors. */
void *memmove(void *dest, const void *src, size_t n);
void *memset(void *s, int c, size_t n);
/* Include a prototype which is not standard in C89 but available on gcc. */
int snprintf(char *str, size_t size, const char *format, ...);


/* Replace the standard strcmp() function with our function */
#define strcmp compareString
/* Replace the standard strlen() function with our function */
#define strlen stringLength
/* Replace the standard strcpy() function with our function */
#define strcpy provided_dangerousStrCpy
/* Replace the standard strncpy() function with our function */
#define strncpy provided_dangerousStrNCpy
/* Replace the standard strdup() function with our function */
#define strdup duplicateString
/* Replace the standard strcasecmp() function with our function */
#define strcasecmp icaseCompareString
/* Replace the standard tolower() function with our function */
#define tolower toLowerChar
/* Replace the standard toupper() function with our function */
#define toupper toUpperChar
/* Replace the standard strcat() function with our function */
#define strcat provided_dangerousStrCat
/* Replace the standard strncat() function with our function */
#define strncat provided_dangerousStrNCat
/* Replace the standard index() function with our function */
#define index indexOfChar
/* Replace the standard strstr() function with our function */
#define strstr indexOfString
/* Replace goto */
#define goto YouGotABigO

#include <MyString.h>
#include <Registry.h>

/* Generate a warning if the result is not used */
extern FILE *fopen(const char *path, const char *mode) __attribute__((warn_unused_result));
extern size_t fread(void *ptr, size_t size, size_t nmemb, FILE *stream) __attribute__((warn_unused_result));
extern size_t fwrite(const void *ptr, size_t size, size_t nmemb, FILE *stream) __attribute__((warn_unused_result));
extern void *malloc(size_t size) __attribute__((warn_unused_result));
extern void *calloc(size_t nmemb, size_t size) __attribute__((warn_unused_result));
extern void *realloc(void *ptr, size_t size) __attribute__((warn_unused_result));
extern char *fgets(char *s, int size, FILE *stream) __attribute__((warn_unused_result));


/** Test if name is specified as one of the command line parameters.
 * @return true if specified, false otherwise
 */
int isSpecified(const char * name);


void Config_init(int argc, char ** argv);

#define BASEPATH "/tmp/facturation"

#endif
