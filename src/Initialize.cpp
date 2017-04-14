#include <cstdio>
#include <cstring>
#include "FileSystem.h"
#include "FileManager.h"
#include "Inode.h"

int createSuperBlock(FILE* fd, int isize, int dsize) {
    SuperBlock superBlock;
    superBlock.s_isize = isize;
    superBlock.s_fsize = sizeof(superBlock)+isize+dsize;
    memset(superBlock.s_ib, 0, 4*sizeof(int));
    memset(superBlock.s_db, 0, 1024*sizeof(int));
    SuperBlock::setBit(superBlock.s_ib, 0, 1);
    SuperBlock::setBit(superBlock.s_ib, 1, 1);
    SuperBlock::setBit(superBlock.s_db, 0, 1);
    fwrite(&superBlock, sizeof(superBlock), 1, fd);
    return 0;
}

int createDiskInodes(FILE* fd, int count) {
    DiskInode diskInode;
    diskInode.d_mode = 0;
    diskInode.d_nlink = 0;
    for(int i=0; i<count; ++i) {
        fwrite(&diskInode, sizeof(diskInode), 1, fd);
    }
    return 0;
}

int createDataBlocks(FILE* fd, int count) {
    char dataBlock[512] = {0};
    for(int i=0; i<count; ++i) {
        fwrite(dataBlock, sizeof(char), 512, fd);
    }
    return 0;
}

int createDirecotries(FILE* fd, int sbSize, int iSize) {
    DiskInode rootDirInode;
    rootDirInode.d_mode = Inode::IFDIR;
    rootDirInode.d_size = (DirectoryEntry::DIRSIZ+4);
    rootDirInode.d_addr[0] = 19;

    fseek(fd, sbSize+sizeof(DiskInode), SEEK_SET);
    fwrite(&rootDirInode, sizeof(DiskInode), 1, fd);

    DirectoryEntry rootDir;
    rootDir.m_ino = 1;

    fseek(fd, sbSize+iSize, SEEK_SET);
    rootDir.m_name[0] = '/';
    rootDir.m_name[1] = '\0';
    fwrite(&rootDir, sizeof(DirectoryEntry), 1, fd);
//    rootDir.m_name[0] = '.';
//    rootDir.m_name[1] = '.';
//    rootDir.m_name[2] = '\0';
//    fwrite(&rootDir, sizeof(DirectoryEntry), 1, fd);

    return 0;
}

int main(int argc, char *argv[])
{
    FILE* imgFile = fopen(argv[1], "w");
    if(imgFile == NULL) {
        printf("Error while create image file.");
        return 1;
    }
    createSuperBlock(imgFile, 80, 32000);
    createDiskInodes(imgFile, 80);
    createDataBlocks(imgFile, 32000);
    createDirecotries(imgFile, 4608, 5120);
    fclose(imgFile);
    return 0;
}
