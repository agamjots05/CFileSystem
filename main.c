#include <stdio.h>
#include "types.h"
#include <string.h>
#include <unistd.h>

#define DISK_SIZE (10 * 1024 * 1024) // This is 10mb of hard disk we can write data into
#define BLOCK_SIZE 512 // bytes per block
#define NUM_INODES 128

FILE *filePointer;

/*
 * Disk layout
 *   block 0  -> superblock at the start of the file
 *   blocks 1 --> inode region
 */
static long inode_byte_offset(int index) {
    return (long)BLOCK_SIZE + (long)index * (long)sizeof(INode);
}

/* Load inode number `index` from the open disk file. */
int read_inode(FILE *fp, int index, INode *out) {
    if (index < 0 || index >= NUM_INODES || out == NULL)
        return -1;
    if (fseek(fp, inode_byte_offset(index), SEEK_SET) != 0)
        return -1;
    if (fread(out, sizeof(INode), 1, fp) != 1)
        return -1;
    return 0;
}

/* Save inode number `index` to the disk file. */
int write_inode(FILE *fp, int index, const INode *in) {
    if (index < 0 || index >= NUM_INODES || in == NULL)
        return -1;
    if (fseek(fp, inode_byte_offset(index), SEEK_SET) != 0)
        return -1;
    if (fwrite(in, sizeof(INode), 1, fp) != 1)
        return -1;
    return 0;
}

/* Call once when formatting: every inode slot is unused (all zeros). */
static int write_empty_inode_table(FILE *fp) {
    INode empty;
    memset(&empty, 0, sizeof(empty));
    for (int i = 0; i < NUM_INODES; i++) {
        if (write_inode(fp, i, &empty) != 0)
            return -1;
    }
    return 0;
}

//This method will take care of init our storage system before user can do anything
int initData(){
    // Create our hard disk to store all of our files
    filePointer = fopen("virtualDisk.bin", "wb");
    if (filePointer == NULL){
        printf("Failed to create virtual disk");
        return -1;
    }
    // This will actually stretch our file to fit the 10MB size we set
    ftruncate(fileno(filePointer), DISK_SIZE);

    // Now we need to add the superblock at the front of this file
    SuperBlock sb;
    sb.totalBlocks = DISK_SIZE / BLOCK_SIZE;
    sb.iNodeCount = NUM_INODES;

    //To find when the first data block starts we would need to first figure out how many blocks all the iNodes are taking and start after the last block being occupied
    int totalINodeBytes = sb.iNodeCount * (int)sizeof(INode);
    int totalINodeBlocksUsed = (totalINodeBytes + BLOCK_SIZE - 1) / BLOCK_SIZE;
    sb.dataBlockStart = totalINodeBlocksUsed + 1;

    if (fseek(filePointer, 0, SEEK_SET) != 0){
        printf("Error with moveing to start of memory");
        fclose(filePointer);
    };
    size_t elementsWritten = fwrite(&sb, sizeof(sb), 1, filePointer);
    if (elementsWritten != 1){
        printf("Error, could only write %zu elements \n", elementsWritten);
        fclose(filePointer);
        return -1;
    }

    /* Reserve on-disk inode slots so create/search can tell free (isUsed==0) from used. */
    if (write_empty_inode_table(filePointer) != 0) {
        printf("Error writing inode table\n");
        fclose(filePointer);
        return -1;
    }

    fclose(filePointer);

    return 1;
}

void promptUser(char title[]){
    printf("%s\n\n", title);
    printf("> Enter file name: ");
}

int main(){
    int choice;
    char fileName[100];
    
    if (initData() == -1) {
        return 1;
    }

    printf("\n=====================================\n");
    printf("      C FILE SYSTEM INTERFACE\n");
    printf("=====================================\n\n");
    
    while(1){
        printf("   1. Create a File \n");
        printf("   2. Search for a File \n");
        printf("   3. Open a File \n");
        printf("   4. Exit\n\n");

        printf("> Enter choice: ");


        // Wait for user to input a number
        if (scanf("%d", &choice) != 1 || choice < 1 || choice > 4) {
            while (getchar() != '\n');
            printf("\nInvalid input. Please enter a number between 1-4.\n");
            continue;
        }

        printf("\n-------------------------------------\n");

        switch (choice) {
            case 1:
                promptUser("Create File");
                printf("\n");
                printf("\n-------------------------------------\n");

                break;
                //TODO: Prob call a function to do this 

            case 2:
                promptUser("Search File");
                printf("\n");
                printf("\n-------------------------------------\n");

                break;
                //TODO: Prob call a function to do this 

            case 3:
                promptUser("Open File");
                printf("\n");
                printf("\n-------------------------------------\n");

                break;
                //TODO: Prob call a function to do this 
                //This method will most likely also call the 'close file' method. Since after the user opens the file and makes some changes, they would need to close it before moving on
            case 4:
                printf("Exiting...\n\n\n");
                return 0;
        }
    }


    return 1;
}
