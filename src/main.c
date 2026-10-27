#include "types.h"
#include "fileOperations/fileOps.h"
#include "filePersistance/filePersist.h"

#include <unistd.h>
#include <string.h>
#include <stdio.h>

int initFileSystem(FileSystem *fs, const char *diskName);
int fileExists(const char *diskName);

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
    char fileName[FILE_NAME_LEN];
    char writeBuf[WRITE_BUF_LEN];
    int choice;

    printf("\n=====================================\n");
    printf("      C FILE SYSTEM INTERFACE\n");
    printf("=====================================\n\n");

    if (fileExists(diskName) == 1){
        while(1){
            printf("An existing file system was found \n");
            printf("1. Continue Where You Left Off?\n");
            printf("2. Reset (Erase hardisk and start from scratch\n)");
            printf("> Enter Choice: ");
            if (scanf("%d",&choice) != 1 || choice < 1 || choice > 2){
                flushLine();
                continue;

            }
            printLine();
            switch (choice){
                case 1:
                    if (loadFileSystem(&fs) == -1) {
                        printf("Error loading file system\n");
                        return -1;
                    }
                    printf("Finished loading file system\n");
                    return 0;
                
                case 2:
                    break;
            }

        }

    }
    if (initFileSystem(&fs, diskName) == -1){
        printf("Error Initializing File System");
        return -1;
    }
    printf("Finished Initializing File System\n");
    return 0;
}
