#include "types.h"
#include "fileOps.h"

#include <unistd.h>
#include <string.h>
#include <stdio.h>


FILE *filePointer;

int initInodes(FILE *filePointer) {
    INode emptyINode;
    memset(&emptyINode, 0, sizeof emptyINode);
    for (int i = 0; i < NUM_INODES; i++) {
        size_t elementsWritten = fwrite(&emptyINode, sizeof(emptyINode), 1, filePointer);
        if (elementsWritten != 1) {
            printf("Error, could only write %zu elements in InitInodes \n", elementsWritten);
            return -1;
        }
    }
    return 1;
}

//This method will take care of init our storage system before user can do anything
int initSuperBlock() {
    filePointer = fopen("virtualDisk.bin", "wb");
    if (filePointer == NULL) {
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
    sb.nextFreeDataBlock = sb.dataBlockStart;

    if (fseek(filePointer, 0, SEEK_SET) != 0) {
        printf("Error with moveing to start of memory");
        fclose(filePointer);
    };
    size_t elementsWritten = fwrite(&sb, sizeof(sb), 1, filePointer);
    if (elementsWritten != 1) {
        printf("Error, could only write %zu elements in initSuperBlock\n", elementsWritten);
        fclose(filePointer);
        return -1;
    }
    // Even though the superblock takes up 12 bytes, we're going to move to byte 512 to start the Inodes for easier math in the future
    fseek(filePointer, BLOCK_SIZE, SEEK_SET);

    initInodes(filePointer);

    fclose(filePointer);


    return 1;
}


void promptUser(char title[]) {
    printf("%s\n\n", title);
    printf("> Enter file name: ");
}

void printLine() {
    printf("\n\n-------------------------------------\n");
}


int main() {
    int choice;
    char fileName[100];

    if (initSuperBlock() == -1) {
        return 1;
    }

    printf("\n=====================================\n");
    printf("      C FILE SYSTEM INTERFACE\n");
    printf("=====================================\n\n");

    while (1) {
        printf("\n");
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

        printLine();

        switch (choice) {
            case 1:
                promptUser("Create File");
                scanf("%31s", fileName);
                createFile(fileName);
                printLine();

                break;

            case 2:
                promptUser("Search File");
                scanf("%31s", fileName);
                searchFile(fileName);
                printLine();

                break;

            case 3:
                promptUser("Open File");
                printLine();

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
