#ifndef FILEPERSIST
#define FILEPERSIST

#include "types.h"


/*
ARGS:
    fs: Filesystem pointer
RETURN:
    Saves the file system by writing all of the changed made in FileSystem pointer to the binary harddisk
*/
int loadFileSystem(FileSystem *fs);
int saveFileSystem(FileSystem *fs);

/*
ARGS:
    sb: Pointer to superblock
DESCRIPTION:
    Checks the datablock stack located in SuperBlock
    If top of the stack is non-zero, pop from stack by decrementing the topDataBlockStack pointer
*/

int getNextFreeDataBlock(SuperBlock *sb);

/*
ARGS:
    sb: Pointer to superBlock
DESCRIPTION:
    Checks the freeINodeStack to see if we have a free INode to populate
    If finds a valid block, pop from stack and returns the un-occupied iNode block.
*/
int getNextFreeINodeBlock(SuperBlock *sb);

/*
ARGS:
    sb: Pointer to superblock
    freeDb: 
DESCRIPTION:
    Internal helper method to find the exact byte location where the data block starts.

*/
long _getOffset(SuperBlock *sb, int freeDb);


#endif
#define FILEPERSIST
