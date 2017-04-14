#include "FileSystem.h"

void SuperBlock::setBit(void* bitmap, int index, int value) {
    char* bitmapChar = (char*) bitmap;
    bitmapChar += index/8;
    int restIndex = index%8;
    if(value == 0)
        *bitmapChar = *bitmapChar & ~(1<<restIndex);
    else
        *bitmapChar = *bitmapChar | (1<<restIndex);
}

bool SuperBlock::getBit(void* bitmap, int index) {
    char* bitmapChar = (char*) bitmap;
    bitmapChar += index/8;
    int restIndex = index % 8;
    return (*bitmapChar & (1<<restIndex));
}

