#ifndef TYPES
#define TYPES

typedef struct {
    int totalBlocks;
    int iNodeCount;
    int dataBlockStart;
} SuperBlock;

#define MAX_BLOCKS_PER_INODE 10
typedef struct {
    char name[32];
    int isUsed;
    int size;
    int dataBlocksUsed[MAX_BLOCKS_PER_INODE];

} INode;

#endif
