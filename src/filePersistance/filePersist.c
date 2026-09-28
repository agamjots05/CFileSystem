#include "types.h"
#include <stdio.h>


// Saves our current FileSystem struct into our virtualDisk binary
int saveFileSystem(FileSystem *fs){
    return 0;
}

int getNextFreeDataBlock(SuperBlock *sb){
    if (sb->topDataBlockStack < 0){
        printf("Error: No Valid Data Block Found\n");
        return -1;
    }

    int validDb = sb->freeDataBlockStack[sb->topDataBlockStack];
    sb->topDataBlockStack--;
    return validDb;
}


int getNextFreeINodeBlock(SuperBlock *sb){
    if (sb->topInodeStack < 0){
        printf("Error: No Valid INode Block Found\n");
        return -1;
    }

    int validInb = sb->freeInodeStack[sb->topInodeStack];
    sb->topInodeStack--;
    return validInb;
}

long _getOffset(SuperBlock *sb, int freeDb){
    return (long)BLOCK_SIZE * (sb->dataBlockStart * freeDb);
}