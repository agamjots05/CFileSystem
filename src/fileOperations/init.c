#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include "types.h"

/*
    We want to util the FileSystem struct where we'll be changing this struct
    save it when we want to write to disk.

    1. Open file in wb mode
    2. Call initSuper block and have it return our superblock
    3. Call initInodes to return arr of Inodes
    4. Call saveFileSystem using our fileSystem pointer


*/
int initFileSystem(FileSystem *fs, const char diskName){
    fs->diskFile = fopen(diskName, "wb");
    if (fs->diskFile == NULL){
        printf("Error creating file \n");
        return 0;
    }
    ftruncate(fileno(fs->diskFile), DISK_SIZE);

    initSuperblock(&fs->sb);

    initInodes(fs->inodeList);

    initRootDirectory(fs);

    saveFileSystem(fs);


    return 1;
}

int fileExists(const char *diskName){
    FILE *fp = fopen(diskName, "rb");
    if (fp == NULL){
        printf("File Doesn't Exist Try Again");
        return 0;
    }
    fclose(fp);
    return 1;
}

void initSuperBlock(SuperBlock *sb){

    sb->totalBlocks = DISK_SIZE / BLOCK_SIZE;
    sb->iNodeCount = NUM_INODES;
    sb->rootInodeNum = 0;

    //To find when the first data block starts we would need to first figure out how many blocks all the iNodes are taking and start after the last block being occupied
    int totalINodeBytes = sb->iNodeCount * (int)sizeof(INode);
    int totalINodeBlocksUsed = (totalINodeBytes + BLOCK_SIZE - 1) / BLOCK_SIZE;
    sb->dataBlockStart = totalINodeBlocksUsed + 1;

    sb->topInodeStack = -1;
    for (int i=0; i < NUM_INODES; i++){
        sb->freeInodeStack[++sb->topInodeStack] = i;
    }
    sb->topDataBlockStack = -1;
    for (int i=0; i < MAX_NUM_DATA_BLOCKS; i++){
        sb->freeDataBlockStack[++sb->topDataBlockStack] = i;
    }
}

void initRootDirectory(FileSystem *fs){

}

void initInodes(INode *iNodeList) {
    for (int i=0;i< sizeof(iNodeList); i++){
        INode empty;
        empty.type = UNUSED;
        empty.isUsed = 0;
        empty.size = 0;
        for (int j=0; j < sizeof(empty.dataBlocksUsed); j++){
            empty.dataBlocksUsed[j] = -1;
        }
        iNodeList[i] = empty;
    }
}