#include <cstdio>
#include "DummyKernel.h"
#include "BufferManager.h"
#include "FileSystem.h"

DummyKernel DummyKernel::instance;
FileManager g_FileManager;

DummyKernel::DummyKernel() {
}

DummyKernel& DummyKernel::Instance() {
    return DummyKernel::instance;
}

void DummyKernel::Initialize(FILE* fd) {
    User user;
    this->m_User = &user;

    this->m_BufferManager = &g_BufferManager;
    this->m_BufferManager->Initialize(fd);

    this->m_FileSystem = &g_FileSystem;
    this->m_FileSystem->Initialize();

    this->m_FileManager = &g_FileManager;
    this->m_FileManager->Initialize();
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

FileManager& DummyKernel::GetFileManager() {
    return *(this->m_FileManager);
}
