#include "types.h"
#include <stdio.h>


// Saves our current FileSystem struct into our virtualDisk binary
int saveFileSystem(FileSystem *fs){


    return 0;
}

FILE* resetFileSystem(const char *diskName){
    FILE* fp = fopen(diskName, "wb");
    if (fp == NULL){
        printf("Error Resetting File");
        return NULL;
    }
    return fp;

}