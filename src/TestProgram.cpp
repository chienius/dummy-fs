#include <iostream>
#include <cstdio>
#include <cstdlib>
#include "DummyKernel.h"
#include "FileManager.h"
#include "Inode.h"
#include "Utility.h"
int main(int argc, char *argv[])
{
    if(argc != 2) {
        printf("Usage: TestProgram imagefile\n");
    }

    // Initialize Kernel
    FILE* fp = fopen(argv[1], "r+");
    DummyKernel::Instance().Initialize(fp);
    FileManager& fm = DummyKernel::Instance().GetFileManager();
    User& us = DummyKernel::Instance().GetUser();

    /*  初始化rootDirInode和用户当前工作目录，以便NameI()正常工作 */
    FileManager& fileMgr = DummyKernel::Instance().GetFileManager();
    fileMgr.rootDirInode = g_InodeTable.IGet(1, FileSystem::ROOTINO);
    fileMgr.rootDirInode->i_flag &= (~Inode::ILOCK);
    
    us.u_cdir = g_InodeTable.IGet(1, FileSystem::ROOTINO);
    us.u_cdir->i_flag &= (~Inode::ILOCK);
    Utility::StringCopy("/", us.u_curdir);
    
    char testFilePath[] = "/test2";
    char testDirPath[] = "/testdir2";
    char testDirTestPath[] = "/testdir2/test3";
    char rootPath[] = "/";
    char testBuffer[]="This is a test string. If you can read this...\nHoorayyyy!!! You are awesome!";
    char testOutBuffer[256];
    
    FileSystem& fileSys = DummyKernel::Instance().GetFileSystem();
    
    fm.Ls(rootPath);
    
    // Create test file
    int f1 = fm.Creat(testFilePath, Inode::IRWXU);
    fileSys.Update();
    fm.Close(f1);
    fm.Ls(rootPath);
    
    // Create test directory
    int f2 = fm.Creat(testDirPath, Inode::IRWXU|Inode::IFDIR);
    fileSys.Update();
    fm.Close(f2);
    fm.Ls(rootPath);
    
    // Read test file
    int f3 = fm.Open(testFilePath, Inode::IRWXU);
    fm.Write(f3, testBuffer, sizeof(testBuffer));
    fileSys.Update();
    fm.Seek(f3, 0, 0);
    fm.Read(f3, testOutBuffer, sizeof(testBuffer));
    fileSys.Update();
    fm.Close(f3);
    fileSys.Update();
    printf("%s\n", testOutBuffer);
    fm.Ls(rootPath);

    // Remove test file
    fm.UnLink(testFilePath);
    
    // Create test
    //int f4 = fm.Creat(testDirTestPath, Inode::IRWXU);
    //fm.Close(f4);
    //fm.Ls(rootPath);
    //fm.Ls(testDirPath);
    
    // Remove test directory
    fm.UnLink(testDirPath);
    fileSys.Update();
    fm.Ls(rootPath);

    fclose(fp);
    return 0;
}
