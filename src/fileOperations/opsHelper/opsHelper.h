#ifndef OPSHELPER
#define OPSHELPER

#include "types.h"
/*
ARGS:
    fs: Pointer to the file system being modified 
DESCRIPTION:
    Returns which datablock was added to be used by the corresponding INode
*/
int _initDirDataBlock(FileSystem *fs);


/*
ARGS:
    dbID: Which datablock is being referenced
    fName: Name of the FIle j
    ftype: Type of file being added
DESCRIPTION:

*/
int addEntryDataBlock(int dbID, char *fName, FileType ftype);


#endif
