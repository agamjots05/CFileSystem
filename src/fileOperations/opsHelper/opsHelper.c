#include "types.h"
#include "../../filePersistance/filePersist.h"
#include "opsHelper.h"
#include <stdio.h>

int _initDirDataBlock(FileSystem *fs){
    SuperBlock *sb = &fs->sb;
    int freeDb = getNextFreeDataBlock(sb);
    if (freeDb == -1){
        printf("Error: No Free Datablocks available.");
        return -1;
    }

    //get offset
    long offSet = _getOffset(sb, freeDb);
    //move to proper location where data block starts and fill up with DirEntries
    if (fseek(fs->diskFile, offSet, SEEK_SET) != 0){
        printf("Error: Failure Seeking to Datablock location");
        return -1;

    }
    for (int i=0; i < (BLOCK_SIZE / sizeof(DirEntry)); i++){
        DirEntry temp;
        temp.iNode = -1;
        temp.name[0] = '\0';
        fwrite(&temp, sizeof(temp), 1, fs->diskFile);
    }

    //save system
    if (saveFileSystem(fs) == -1) return -1;
    return freeDb;
}
int addEntryDataBlock(int dbID, char *fName, FileType ftype){
    (void)dbID;
    (void)fName;
    (void)ftype;
    return -1;
}

