#include "types.h"
#include "fileOps.h"

#include <unistd.h>
#include <string.h>
#include <stdio.h>

const char* diskName = "virtualDisk.bin";
const int INPUT_SIZE = 10;
const int FILE_NAME_LEN = 100;
const int WRITE_BUF_LEN = 512;
FILE *filePointer;  






void promptUser(char title[]) {
    printf("%s\n\n", title);
    printf("> Enter file name: ");
}

void printLine() {
    printf("\n\n-------------------------------------\n");
}


static void flushLine(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {
    }
}

int main() {
    FileSystem fs;

    char initChoice[INPUT_SIZE];
    char fileName[FILE_NAME_LEN];
    char writeBuf[WRITE_BUF_LEN];
    int choice;
    

    if (fileExists(diskName)){
        printf("An existing file system was found \n");
        printf("1. Continue Where You Left Off?\n");
        printf("2. Reset (Erase hardisk and start from scratch\n)");


    }

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
        printf("   4. Reset Disk \n");
        printf("   5. Exit\n\n");

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
                scanf("%31s", fileName);
                flushLine();
                printf("> Text to store (max %d chars, one line):\n> ", BLOCK_SIZE);
                if (fgets(writeBuf, sizeof writeBuf, stdin) == NULL) {
                    writeBuf[0] = '\0';
                }
                writeToFile(fileName, writeBuf, strlen(writeBuf));
                printLine();

                break;
                //TODO: Prob call a function to do this
                //This method will most likely also call the 'close file' method. Since after the user opens the file and makes some changes, they would need to close it before moving on
            case 5:
                printf("Resetting Disk...\n");
                resetFileSystem("virtualDisk.bin");
            case 4:
                printf("Exiting...\n\n\n");
                return 0;
        }
    }


    return 1;
}
