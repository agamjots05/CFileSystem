#include "types.h"
#include <stdio.h>

int loadFileSystem(FileSystem *fs){
    fs->diskFile = fopen("virtualDisk.bin", "rb+");
    if (fs->diskFile == NULL) return -1;
    fseek(fs->diskFile, 0, SEEK_SET);
    if (fread(&fs->sb, sizeof(SuperBlock), 1, fs->diskFile) != 1) return -1;

    // Adding an offset of size BLOCK_SIZE since first INode lives in Block 1 
    // Block 0 is being occupied by SuperBlock.
    fseek(fs->diskFile,BLOCK_SIZE, SEEK_SET);
    if (fread(fs->inodeList, sizeof(INode), NUM_INODES, fs->diskFile) != NUM_INODES) return -1;

    return 1;
}

int saveFileSystem(FileSystem *fs){
    if (fs == NULL || fs->diskFile == NULL || fseek(fs->diskFile, 0, SEEK_SET) != 0) return -1;

    if (fwrite(&fs->sb, sizeof(SuperBlock),1, fs->diskFile) != 1) return -1;

    if (fseek(fs->diskFile, BLOCK_SIZE, SEEK_SET) != 0 ||
        fwrite(fs->inodeList, sizeof(INode), NUM_INODES, fs->diskFile) != NUM_INODES) return -1;

    return fflush(fs->diskFile) == 0 ? 1 : -1;
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
    return (long)BLOCK_SIZE * (sb->dataBlockStart + freeDb);
}
