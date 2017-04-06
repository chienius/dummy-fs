#include "BufferManager.h"
#include "User.h"
#include "FileSystem.h"
#ifndef DUMMYKERNEL_H
#define DUMMYKERNEL_H

class DummyKernel
{
public:
    DummyKernel();
    static DummyKernel& Instance();
    void Initialize(FILE* fd);

    BufferManager& GetBufferManager();
    User& GetUser();
    FileSystem& GetFileSystem();
private:
    static DummyKernel instance;    // DummyKernel instance
    BufferManager* m_BufferManager;
    User* m_User;
    FileSystem* m_FileSystem;
};

#endif /* DUMMYKERNEL_H */
