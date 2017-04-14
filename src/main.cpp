//
//  main.cpp
//  DummyFS
//
//  Created by Archie Lu on 2017/4/8.
//  Copyright © 2017年 Archibald Lu. All rights reserved.
//

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include "DummyKernel.h"
#include "FileManager.h"
#include "Inode.h"
#include "Utility.h"
#include "CommandUtility.hpp"
#include <cstring>

int main(int argc, const char * argv[]) {
    if(argc != 2) {
        printf("Usage: DummyFS imagefile\n");
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

    printf("Dummy Kernel has been initialized.\n");
    
//    CommandUtility::load("test-text2.txt", "/text2.txt");
//    CommandUtility::save("/text2.txt", "output/text2.txt");
//    
//    CommandUtility::load("test-text1.txt", "/text1.txt");
//    CommandUtility::save("/text1.txt", "output/text1.txt");
    
    //CommandUtility::unlink("/text2.txt");
    //return 0;
    while(true) {
        using namespace std;
        printf("> ");
        us.u_error = 0;
        char command[128];
        cin >> command;
        if(strcmp(command, "shutdown") == 0) {
            break;
        }
        if (strcmp(command, "load") == 0) {
            char source[128], dest[128];
            cin >> source >> dest;
            cout << "Loading local file " << source << " to " << dest << endl;
            CommandUtility::load(source, dest);
        } else if(strcmp(command, "unlink") == 0) {
            char path[128];
            cin >> path;
            cout << "Removing file " << path << endl;
            CommandUtility::unlink(path);
        } else if(strcmp(command, "mkdir") == 0) {
            char path[128];
            cin >> path;
            cout << "Making directory " << path << endl;
            CommandUtility::mkdir(path);
        } else if(strcmp(command, "save") == 0) {
            char source[128], dest[128];
            cin >> source >> dest;
            cout << "Saving " << source << " to local file " << dest << endl;
            CommandUtility::save(source, dest);
        } else if(strcmp(command, "ls") == 0) {
            char path[128];
            cin >> path;
            cout << "Listing " << path << endl;
            CommandUtility::ls(path);
        } else if(strcmp(command, "rmdir") == 0) {
            char path[128];
            cin >> path;
            cout << "Removing directory " << path << endl;
            CommandUtility::rmdir(path);
        } else {
            printf("No such command.\n");
        }
    }
    
    return 0;
}
