#ifndef TYPES
#define TYPES

#define DISK_SIZE (10 * 1024 * 1024) // This is 10mb of hard disk we can write data into
#define BLOCK_SIZE 512 // we can have each block start with 512 bytes
#define NUM_INODES 128
#define MAX_BLOCKS_PER_INODE 10
#define MAX_NUM_DATA_BLOCKS (NUM_INODES * MAX_BLOCKS_PER_INODE)

// Each iNode will be one of the 2 types either a directory or a file.
typedef enum { FILE_TYPE, DIR_TYPE } FileType;


typedef struct {
    SuperBlock sb;
    INode inodeList[NUM_INODES];
    FILE *diskFile;
}FileSystem;

// A directory is a key value pair where the name will correspond to an iNode.
typedef struct {
    char name[32];
    int iNode;
}DirEntry;



typedef struct {
    int totalBlocks;
    int iNodeCount;
    int dataBlockStart;
    int rootInodeNum;

    //Keep Track of Next Valid Inode
    int freeInodeStack[NUM_INODES];
    int topInodeStack;

    //Keep Track of Next Valid Data Block
    int freeDataBlockStack[MAX_NUM_DATA_BLOCKS];
    int topDataBlockStack;

} SuperBlock;


typedef struct {
    FileType type; 
    int isUsed;
    int size;
    int dataBlocksUsed[MAX_BLOCKS_PER_INODE];
    
} INode;

#endif
