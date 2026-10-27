#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include "types.h"

#include "filePersistance/filePersist.h"
#include "fileOperations/opsHelper/opsHelper.h"

int initFileSystem(FileSystem *fs, const char *diskName){
    fs->diskFile = fopen(diskName, "wb");
    if (fs->diskFile == NULL){
        printf("Error creating file \n");
        return -1;
    }
    if (ftruncate(fileno(fs->diskFile), DISK_SIZE) != 0) {
        fclose(fs->diskFile);
        fs->diskFile = NULL;
        return -1;
    }

    initSuperBlock(&fs->sb);

    if (initInodes(fs, fs->inodeList) == -1){
        printf("Fatal: Failed to init file system\n");
        fclose(fs->diskFile);
        fs->diskFile = NULL;
        return -1;
    };

    if (saveFileSystem(fs) == -1) return -1;
    return 1;
}

int fileExists(const char *diskName){
    FILE *fp = fopen(diskName, "rb");
    if (fp == NULL){
        printf("File Doesn't Exist Try Again\n");
        return -1;
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
int initRootDirectory(FileSystem *fs, INode *rootNode){
    int db = _initDirDataBlock(fs);
    if (db == -1){
        printf("Fatal: Failed to Init Inodes");
        return -1;
    }
    rootNode->type = DIR_TYPE;
    rootNode->isUsed = 1;
    rootNode->size = 0;
    for (int i = 0; i < MAX_BLOCKS_PER_INODE; i++) rootNode->dataBlocksUsed[i] = -1;
    rootNode->dataBlocksUsed[0] = db;
    return 1;
}

int initInodes(FileSystem *fs, INode *iNodeList) {
    for (int i=0; i < NUM_INODES; i++){
        INode empty = {0};
        if (i == 0){
            if(initRootDirectory(fs, &empty) == -1){
                printf("Fatal: Failure to Init Inodes \n");
                return -1;
            }
        }
        else{
            empty.type = UNUSED;
            empty.isUsed = 0;
            empty.size = 0;
            for (int j=0; j < MAX_BLOCKS_PER_INODE; j++){
                empty.dataBlocksUsed[j] = -1;
            }
        }
        iNodeList[i] = empty;
    }
    return 1;
}
