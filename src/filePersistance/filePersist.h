#ifndef FILEPERSIST
#define FILEPERSIST

#include "types.h"

int saveFileSystem(FileSystem *fs);

int getNextFreeDataBlock(SuperBlock *sb);

int getNextFreeINodeBlock(SuperBlock *sb);


#endif
#define FILEPERSIST