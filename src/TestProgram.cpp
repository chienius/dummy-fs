#include <cstdio>
#include <cstdlib>
#include "DummyKernel.h"
#include "FileManager.h"
#include "Inode.h"

int main(int argc, char *argv[])
{
    if(argc != 2) {
        printf("Usage: TestProgram imagefile\n");
    }

    // Initialize Kernel
    FILE* fp = fopen(argv[1], "r+");
    DummyKernel::Instance().Initialize(fp);
    FileManager& fm = DummyKernel::Instance().GetFileManager();

    char testFilePath[] = "/test";
    char testDirPath[] = "/testDir";
    char rootPath[] = "/";

    fm.Ls(rootPath);
    // Create test file
    //fm.Creat(testFilePath, Inode::IRWXU);
    // Create test directory
    //fm.Creat(testFilePath, Inode::IRWXU|Inode::IFDIR);
    // Ls
    //fm.Ls(rootPath);
    // Read test file
    // Remove test file
    // Ls
    // Remove test directory
    // Ls

    return 0;
}
