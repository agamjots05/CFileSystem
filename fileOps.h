#ifndef FILEOPS
#define FILEOPS

#include "types.h"

#include <stdio.h>

int findFile(const char *name, FILE *fp);
int createFile(const char *name);
void searchFile(const char *name);
int openFile(const char *name);

    
#endif // !FILEOPS
#define FIlEOPS
