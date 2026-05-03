#include "fileOps.h"
#include "types.h"

#include <stdio.h>
#include <string.h>


int findFile(const char *name, FILE *fp) {
    INode temp;

    //Move to the first INode
    fseek(fp, BLOCK_SIZE, SEEK_SET);

    for (int i = 0; i < NUM_INODES; i++){
        if (fread(&temp, sizeof(temp), 1, fp) != 1){
            printf("Error Reading Block in FindFile \n");
            return -1;
        };
        if (temp.isUsed && (strcmp(name, temp.name) == 0)){
            printf("Found File \n");
            return i;
        }
    }
    return -1;
}
int createFile(const char *name){
    FILE *fp = fopen("virtualDisk.bin", "rb+");

    // 0. First get super block data
    SuperBlock sb;
    fseek(fp, 0, SEEK_SET);
    if (fread(&sb, sizeof(SuperBlock), 1, fp) != 1){
        printf("Error Reading SuperBlock in CreateFile \n");
        fclose(fp);
        return -1;
    }


    //1. Find if duplicate fileName exists
    fseek(fp, BLOCK_SIZE, SEEK_SET);

    printf("Checking For Duplicate File \n");
    if (findFile(name, fp) != -1){
        printf("Duplicate file found. Please Try Again \n");
        fclose(fp);
        return -1;
    }

    // since we have just performed a search, we need to move our file pointer back at the first INode location
    fseek(fp, BLOCK_SIZE, SEEK_SET);

    //2. Find first valid Inode to place data in
    INode temp;
    int iNodeLoc = -1;
    for (int i = 0; i < NUM_INODES; i++){
        if (fread(&temp, sizeof(temp), 1, fp) != 1){
            printf("Error Reading Block in CreateFile \n");
            fclose(fp);
            return -1;
        };
        if (!temp.isUsed){
            printf("Found First Valid INode to create file \n");
            iNodeLoc = i;
            break;
        }
    }
    if (iNodeLoc == -1){
        printf("Disk full: No INodes available \n");
        fclose(fp);
        return -1;
    }

    //4. Fill in props of INode
    strncpy(temp.name, name, 31);
    temp.isUsed = 1;
    temp.size = 0;

    int assignedDataBlock = sb.nextFreeDataBlock;
    temp.dataBlocksUsed[0] = assignedDataBlock;
    sb.nextFreeDataBlock++;

    //5. update hard virtualDisk adding new file created in memory
    fseek(fp,BLOCK_SIZE + (iNodeLoc * sizeof(INode)), SEEK_SET);
    fwrite(&temp, sizeof(INode), 1, fp);

    //6. update disk with new superblock since we updated the nextFreeDataBlock
    fseek(fp, 0, SEEK_SET);
    fwrite(&sb, sizeof(SuperBlock), 1, fp);
    printf("Successfully added file to disk \n");
    fclose(fp);
    return 1;
}
