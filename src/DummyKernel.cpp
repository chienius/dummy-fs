#include <cstdio>
#include "DummyKernel.h"
#include "BufferManager.h"
#include "FileSystem.h"

DummyKernel::DummyKernel() {
}

void DummyKernel::Initialize(FILE* fd) {
    User user;
    this->m_User = &user;

    this->m_BufferManager = &g_BufferManager;
    this->m_BufferManager->Initialize(fd);

    this->m_FileSystem = &g_FileSystem;
    this->m_FileSystem->Initialize();
}

FileSystem& DummyKernel::GetFileSystem() {
    return *(this->m_FileSystem);
}

User& DummyKernel::GetUser() {
    return *(this->m_User);
}

BufferManager& DummyKernel::GetBufferManager() {
    return *(this->m_BufferManager);
}
