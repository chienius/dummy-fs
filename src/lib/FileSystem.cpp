#include "DummyKernel.h"
#include "Utility.h"
#include <stdexcept>

SuperBlock g_spb;
FileSystem g_FileSystem;

void FileSystem::Initialize() {
    this->m_BufferManager = &(DummyKernel::Instance().GetBufferManager());
    this->updlock = 0;
    this->LoadSuperBlock();
}

void FileSystem::LoadSuperBlock()
{
    BufferManager& bufMgr = DummyKernel::Instance().GetBufferManager();
    Buf* pBuf;

    for (int i = 0; i < 9; i++)
    {
        int* p = (int *)&g_spb + i * 128;

        pBuf = bufMgr.Bread(1, FileSystem::SUPER_BLOCK_SECTOR_NUMBER + i);

        Utility::DWordCopy((int *)pBuf->b_addr, p, 128);

        bufMgr.Brelse(pBuf);
    }

    g_spb.s_flock = 0;
    g_spb.s_ilock = 0;
    g_spb.s_ronly = 0;
}

void FileSystem::Update()
{
    int i;
    SuperBlock* sb;
    Buf* pBuf;

    /* 另一进程正在进行同步，则直接返回 */
    if(this->updlock)
    {
        return;
    }

    /* 设置Update()函数的互斥锁，防止其它进程重入 */
    this->updlock++;

    /* 同步SuperBlock到磁盘 */
    sb = &g_spb;

    /* 如果该SuperBlock内存副本没有被修改，直接管理inode和空闲盘块被上锁或该文件系统是只读文件系统 */
    if(sb->s_fmod == 0 || sb->s_ilock != 0 || sb->s_flock != 0 || sb->s_ronly != 0)
    {
        return;
    }

    /* 清SuperBlock修改标志 */
    sb->s_fmod = 0;

    /* 
     * 为将要写回到磁盘上去的SuperBlock申请一块缓存，由于缓存块大小为512字节，
     * SuperBlock大小为1024字节，占据2个连续的扇区，所以需要2次写入操作。
     */
    for(int j = 0; j < 9; j++)
    {
        /* 第一次p指向SuperBlock的第0字节，第二次p指向第512字节 */
        int* p = (int *)sb + j * 128;

        /* 将要写入到设备dev上的SUPER_BLOCK_SECTOR_NUMBER + j扇区中去 */
        pBuf = this->m_BufferManager->GetBlk(1, FileSystem::SUPER_BLOCK_SECTOR_NUMBER + j);

        /* 将SuperBlock中第0 - 511字节写入缓存区 */
        Utility::DWordCopy(p, (int *)pBuf->b_addr, 128);

        /* 将缓冲区中的数据写到磁盘上 */
        this->m_BufferManager->Bwrite(pBuf);
    }

    /* TODO 同步修改过的内存Inode到对应外存Inode */
    //g_InodeTable.UpdateInodeTable();

    /* 清除Update()函数锁 */
    this->updlock = 0;
}

Inode* FileSystem::IAlloc(short dev)
{
    SuperBlock* sb;
    Buf* pBuf;
    Inode* pNode;
    int ino;	/* 分配到的空闲外存Inode编号 */

    /* 获取相应设备的SuperBlock内存副本 */
    sb = &g_spb;

    int* ib = sb->s_ib;
    ino = -1;
    for(int i=0; i<FileSystem::INODE_ZONE_SIZE*FileSystem::INODE_NUMBER_PER_SECTOR; ++i) {
        if(SuperBlock::getBit(sb->s_ib, i) == 0) {
            ino = i;
            break;
        }
    }
    if(ino < 0) {
        return NULL;
    }

    /* 
     * 上面部分已经保证，除非系统中没有可用外存Inode，
     * 否则空闲Inode索引表中必定会记录可用外存Inode的编号。
     */
    while(true)
    {
        /* 从索引表“栈顶”获取空闲外存Inode编号 */

        /* 将空闲Inode读入内存 */
        pNode = g_InodeTable.IGet(dev, ino);
        /* 未能分配到内存inode */
        if(NULL == pNode)
        {
            return NULL;
        }

        /* 如果该Inode空闲,清空Inode中的数据 */
        if(0 == pNode->i_mode)
        {
            pNode->Clean();
            /* 设置SuperBlock被修改标志 */
            sb->s_fmod = 1;
            SuperBlock::setBit(sb->s_ib, ino, 1);
            return pNode;
        }
        else	/* 如果该Inode已被占用 */
        {
            g_InodeTable.IPut(pNode);
            continue;	/* while循环 */
        }
    }
    return NULL;	/* GCC likes it! */
}

void FileSystem::IFree(short dev, int number)
{
    SuperBlock* sb;

    sb = &g_spb;	/* 获取相应设备的SuperBlock内存副本 */

    SuperBlock::setBit(sb->s_ib, number, 0);

    /* 设置SuperBlock被修改标志 */
    sb->s_fmod = 1;
}

Buf* FileSystem::Alloc(short dev)
{
    int blkno;	/* 分配到的空闲磁盘块编号 */
    SuperBlock* sb;
    Buf* pBuf;

    /* 获取SuperBlock对象的内存副本 */
    sb = &g_spb;

    blkno = -1;
    for(int i=0; i<FileSystem::DATA_ZONE_SIZE; ++i) {
        if(SuperBlock::getBit(sb->s_db, i) == 0) {
            blkno = i;
            break;
        }
    }
    if(blkno<0) {
        return NULL;
    }

    blkno += FileSystem::DATA_ZONE_START_SECTOR;

    /* 普通情况下成功分配到一空闲磁盘块 */
    pBuf = this->m_BufferManager->GetBlk(dev, blkno);	/* 为该磁盘块申请缓存 */
    this->m_BufferManager->ClrBuf(pBuf);	/* 清空缓存中的数据 */
    sb->s_fmod = 1;	/* 设置SuperBlock被修改标志 */
    SuperBlock::setBit(sb->s_db, blkno, 1);

    return pBuf;
}

void FileSystem::Free(short dev, int blkno)
{
    SuperBlock* sb;
    Buf* pBuf;
    User& u = DummyKernel::Instance().GetUser();

    sb = &g_spb;

    SuperBlock::setBit(sb->s_db, blkno-FileSystem::DATA_ZONE_START_SECTOR, 0);

    sb->s_fmod = 1;
}
