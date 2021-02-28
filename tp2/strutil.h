#ifndef STRUTIL_H
#define STRUTIL_H

#include <stddef.h>


char** split(const char* str, char sep);


char* join(char** strv, char sep);
void free_strv(char* strv[]);


#endif  // STRUTIL_H

