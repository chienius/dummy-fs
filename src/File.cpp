#include "File.h"
#include "Utility.h"	//for NULL
#include "DummyKernel.h"

/*==============================class File===================================*/
File::File()
{
    this->f_count = 0;
    this->f_flag = 0;
    this->f_offset = 0;
    this->f_inode = NULL;
}

File::~File()
{
    //nothing to do here
}

/*==============================class OpenFiles===================================*/
OpenFiles::OpenFiles()
{
    for(int i=0; i<OpenFiles::NOFILES; i++) {
        this->ProcessOpenFileTable[i] = NULL;
    }
}

OpenFiles::~OpenFiles()
{
}

int OpenFiles::AllocFreeSlot()
{
    int i;
    User& u = DummyKernel::Instance().GetUser();

    for(i = 0; i < OpenFiles::NOFILES; i++)
    {
        /* 进程打开文件描述符表中找到空闲项，则返回之 */
        if(this->ProcessOpenFileTable[i] == NULL)
        {
            u.u_eax_ar0 = i;
            return i;
        }
    }

    printf("Alloc Free File Slot Failed. @OpenFiles::AllocFreeSlot");
    return -1;
}

int OpenFiles::Clone(int fd)
{
    return 0;
}

File* OpenFiles::GetF(int fd)
{
    File* pFile;

    /* 如果打开文件描述符的值超出了范围 */
    if(fd < 0 || fd >= OpenFiles::NOFILES)
    {
        printf("Open File Failed. OFR. @OpenFiles::GetF");
        return NULL;
    }

    pFile = this->ProcessOpenFileTable[fd];
    if(pFile == NULL)
    {
        printf("Open File Failed. Empty. @OpenFiles::GetF");
    }

    return pFile;	/* 即使pFile==NULL也返回它，由调用GetF的函数来判断返回值 */
}

void OpenFiles::SetF(int fd, File* pFile)
{
    if(fd < 0 || fd >= OpenFiles::NOFILES)
    {
        return;
    }
    /* 进程打开文件描述符指向系统打开文件表中相应的File结构 */
    this->ProcessOpenFileTable[fd] = pFile;
}

/*==============================class IOParameter===================================*/
IOParameter::IOParameter()
{
    this->m_Base = 0;
    this->m_Count = 0;
    this->m_Offset = 0;
}

IOParameter::~IOParameter()
{
    //nothing to do here
}

