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
            return i;
        }
    }
    return -1;
}
int createFile(const char *name){
    FILE *fp = fopen("virtualDisk.bin", "rb+");
    if (fp == NULL) {
        printf("Error opening virtual disk in createFile.\n");
        return -1;
    }

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

    // printf("Checking For Duplicate File \n");
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
            //  printf("Found First Valid INode to create file \n");
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
    temp.name[31] = '\0';
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

void searchFile(const char *name){
    FILE *fp = fopen("virtualDisk.bin", "rb");
    if (fp == NULL){
        printf("Error opening virtual disk in searchFile.\n");
        return;
    }

    int inodeIndex = findFile(name, fp);

    printf("Searching for file: %s\n", name);
    if (inodeIndex == -1){
        printf("File not found.\n");
        fclose(fp);
        return;
    }
    printf("File found at inode table index: %d\n", inodeIndex);
    fclose(fp);
}

int writeToFile(const char *name, const char *data, size_t len) {
    FILE *fp = fopen("virtualDisk.bin", "rb+");
    if (fp == NULL) {
        printf("Error opening virtual disk\n");
        return -1;
    }

    int idx = findFile(name, fp);
    if (idx < 0) {
        printf("File not found\n");
        fclose(fp);
        return -1;
    }

    INode in;
    if (fseek(fp, BLOCK_SIZE + idx * (long)sizeof(INode), SEEK_SET) != 0 ||
        fread(&in, sizeof(in), 1, fp) != 1) {
        printf("Error reading inode\n");
        fclose(fp);
        return -1;
    }

    int block = in.dataBlocksUsed[0];
    if (block == 0) {
        printf("File has no data block assigned\n");
        fclose(fp);
        return -1;
    }

    size_t n = len;
    if (n > (size_t)BLOCK_SIZE) {
        printf("Only first %d bytes are stored\n", BLOCK_SIZE);
        n = (size_t)BLOCK_SIZE;
    }

    long off = (long)block * BLOCK_SIZE;
    if (fseek(fp, off, SEEK_SET) != 0 || fwrite(data, 1, n, fp) != n) {
        printf("Error writing file data\n");
        fclose(fp);
        return -1;
    }

    in.size = (int)n;
    if (fseek(fp, BLOCK_SIZE + idx * (long)sizeof(INode), SEEK_SET) != 0 ||
        fwrite(&in, sizeof(in), 1, fp) != 1) {
        printf("Error saving inode\n");
        fclose(fp);
        return -1;
    }

    printf("Wrote %zu byte(s) to \"%s\"\n", n, name);
    fclose(fp);
    return 0;
}
