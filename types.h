#ifndef TYPES
#define TYPES

#define DISK_SIZE (10 * 1024 * 1024) // This is 10mb of hard disk we can write data into
#define BLOCK_SIZE 512 // we can have each block start with 512 bytes
#define NUM_INODES 128

typedef struct {
    int totalBlocks;
    int iNodeCount;
    int dataBlockStart;
    int nextFreeDataBlock;
} SuperBlock;

#define MAX_BLOCKS_PER_INODE 10
typedef struct {
    char name[32];
    int isUsed;
    int size;
    int dataBlocksUsed[MAX_BLOCKS_PER_INODE];

} INode;

#endif
