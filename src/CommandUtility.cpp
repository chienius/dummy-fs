//
//  CommandUtility.cpp
//  DummyFS
//
//  Created by Archie Lu on 2017/4/10.
//  Copyright © 2017年 Archibald Lu. All rights reserved.
//

#include "CommandUtility.hpp"
#include "DummyKernel.h"
#include "FileManager.h"
#include "Inode.h"
#include <stdio.h>

void CommandUtility::load(char* src, char* dest) {
    FileManager& fileMgr = DummyKernel::Instance().GetFileManager();
    FileSystem& fileSys = DummyKernel::Instance().GetFileSystem();

    
    int fs_fd = fileMgr.Creat(dest, Inode::IRWXU);
    if(fs_fd<0) {
        printf("Open file on DummyFS failed.\n");
        return;
    }
    FILE* fd = fopen(src, "r");
    if(fd == NULL) {
        printf("Open local file failed.\n");
        return;
    }
    
    char buffer[512];
    while(true) {
        int read_size = (int)fread(buffer, sizeof(char), 512, fd);
        if(read_size == 0) {
            break;
        }
        fileMgr.Write(fs_fd, buffer, read_size);
    }
    fileMgr.Close(fs_fd);
    fclose(fd);
    fileSys.Update();
    
    printf("Loaded.\n");
    return;
}

void CommandUtility::save(char* src, char* dest) {
    FileManager& fileMgr = DummyKernel::Instance().GetFileManager();
    FileSystem& fileSys = DummyKernel::Instance().GetFileSystem();

    
    int fs_fd = fileMgr.Open(src, (File::FREAD | File::FWRITE));
    if(fs_fd<0) {
        printf("Open file on DummyFS failed.\n");
        return;
    }
    FILE* fd = fopen(dest, "w");
    if(fd == NULL) {
        printf("Open local file failed.\n");
        return;
    }
    
    char buffer[512];
    while(true) {
        int read_size = (int)fileMgr.Read(fs_fd, buffer, 512);
        if(read_size == 0) {
            break;
        }
        fwrite(buffer, sizeof(char), read_size, fd);
    }
    fileMgr.Close(fs_fd);
    fclose(fd);
    fileSys.Update();

    printf("Saved.\n");
    return;
}

void CommandUtility::ls(char* path) {
    FileManager& fileMgr = DummyKernel::Instance().GetFileManager();

    fileMgr.Ls(path);
    
    return;
}

void CommandUtility::mkdir(char* path) {
    FileManager& fileMgr = DummyKernel::Instance().GetFileManager();
    FileSystem& fileSys = DummyKernel::Instance().GetFileSystem();
    User& u = DummyKernel::Instance().GetUser();
    
    //fileMgr.MakNode(Inode::IFDIR);
    u.u_dirp = path;
    
    Inode* pInode = fileMgr.NameI(FileManager::NextChar, FileManager::CREATE);
    if ( NULL == pInode )
    {
        if(u.u_error)
            return;
        /* 创建Inode */
        pInode = fileMgr.MakNode( Inode::IFDIR );
        /* 创建失败 */
        if ( NULL == pInode )
        {
            return;
        }
        fileMgr.m_InodeTable->IPut(pInode);
    }
    else
    {
        /* Directory Exists */
        printf("%s already exists.\n", path);
        fileMgr.m_InodeTable->IPut(pInode);
    }

    
    fileSys.Update();
    return;
}

void CommandUtility::unlink(char* path) {
    FileManager& fileMgr = DummyKernel::Instance().GetFileManager();
    FileSystem& fileSys = DummyKernel::Instance().GetFileSystem();
    User& u = DummyKernel::Instance().GetUser();
    
    u.u_dirp = path;
    
    Inode* pInode = fileMgr.NameI(FileManager::NextChar, FileManager::OPEN);
    
    if(pInode == NULL) {
        return;
    }
    
    if((pInode->i_mode & Inode::IFMT)==Inode::IFDIR ) {
        fileMgr.m_InodeTable->IPut(pInode);
        printf("Not a file. \n");
        return;
    }
    fileMgr.m_InodeTable->IPut(pInode);
    
    fileMgr.UnLink(path);
    fileSys.Update();
    return;
}

void CommandUtility::rmdir(char* path) {
    FileManager& fileMgr = DummyKernel::Instance().GetFileManager();
    FileSystem& fileSys = DummyKernel::Instance().GetFileSystem();
    User& u = DummyKernel::Instance().GetUser();

    if(!fileMgr.IsDirEmpty(path)) {
        printf("Directory not empty.\n");
        return;
    }
    
    fileMgr.UnLink(path);
    fileSys.Update();
    return;
}
