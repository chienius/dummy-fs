#include <cstdio>
#include <cstring>
#include <unistd.h>
#include "DummyKernel.h"
#include "BufferManager.h"

BufferManager g_BufferManager;

void BufferManager::Initialize(FILE* fd)
{
    this->imgFd = fd;
    int i;
    Buf* bp;

    this->bFreeList.b_forw = this->bFreeList.b_back = &(this->bFreeList);
    this->bFreeList.av_forw = this->bFreeList.av_back = &(this->bFreeList);

    for(i = 0; i < NBUF; i++)
    {
        bp = &(this->m_Buf[i]);
        bp->b_dev = -1;
        bp->b_addr = this->Buffer[i];
        /* 初始化NODEV队列 */
        bp->b_back = &(this->bFreeList);
        bp->b_forw = this->bFreeList.b_forw;
        this->bFreeList.b_forw->b_back = bp;
        this->bFreeList.b_forw = bp;
        /* 初始化自由队列 */
        bp->b_flags = Buf::B_BUSY;
        Brelse(bp);
    }
    return;
}
Buf* BufferManager::GetBlk(short dev, int blkno)
{
    Buf* bp;
    Devtab* dp = &(this->devtab);
    /* 
     * 如果设备队列中已经存在相应缓存，则返回该缓存；
     * 否则从自由队列中分配新的缓存用于字符块读写。
     */
loop:
    /* 首先在该设备队列中搜索是否有相应的缓存 */
    for(bp = dp->b_forw; bp != (Buf *)dp; bp = bp->b_forw)
    {
        /* 不是要找的缓存，则继续 */
        if(bp->b_blkno != blkno || bp->b_dev != dev)
            continue;

        /* 从自由队列中抽取出来 */
        this->NotAvail(bp);
        return bp;
    }

    /* 如果自由队列为空 */
    if(this->bFreeList.av_forw == &this->bFreeList)
    {
        this->bFreeList.b_flags |= Buf::B_WANTED;
        // Need some sleep
        sleep(1);
        goto loop;
    }

    /* 取自由队列第一个空闲块 */
    bp = this->bFreeList.av_forw;
    this->NotAvail(bp);

    /* 注意: 这里清除了所有其他位，只设了B_BUSY */
    bp->b_flags = Buf::B_BUSY;

    /* 从原设备队列中抽出 */
    bp->b_back->b_forw = bp->b_forw;
    bp->b_forw->b_back = bp->b_back;
    /* 加入新的设备队列 */
    bp->b_forw = dp->b_forw;
    bp->b_back = (Buf *)dp;
    dp->b_forw->b_back = bp;
    dp->b_forw = bp;

    bp->b_dev = dev;
    bp->b_blkno = blkno;
    return bp;
}

void BufferManager::Brelse(Buf* bp)
{
    /* 如果有进程正在等待分配自由队列中的缓存，则唤醒相应进程 */
    if(this->bFreeList.b_flags & Buf::B_WANTED)
    {
        /* 清楚B_WANTED标志，表示已有空闲缓存 */
        this->bFreeList.b_flags &= (~Buf::B_WANTED);
    }

    /* 注意以下操作并没有清除B_DELWRI、B_WRITE、B_READ、B_DONE标志
     * B_DELWRI表示虽然将该控制块释放到自由队列里面，但是有可能还没有些到磁盘上。
     * B_DONE则是指该缓存的内容正确地反映了存储在或应存储在磁盘上的信息 
     */
    bp->b_flags &= ~(Buf::B_WANTED | Buf::B_BUSY | Buf::B_ASYNC);
    (this->bFreeList.av_back)->av_forw = bp;
    bp->av_back = this->bFreeList.av_back;
    bp->av_forw = &(this->bFreeList);
    this->bFreeList.av_back = bp;

    return;
}


Buf* BufferManager::Bread(short dev, int blkno)
{
    Buf* bp;
    /* 根据设备号，字符块号申请缓存 */
    bp = this->GetBlk(dev, blkno);
    /* 如果在设备队列中找到所需缓存，即B_DONE已设置，就不需进行I/O操作 */
    if(bp->b_flags & Buf::B_DONE)
    {
        return bp;
    }
    /* 没有找到相应缓存，构成I/O读请求块 */
    bp->b_flags |= Buf::B_READ;
    bp->b_wcount = BufferManager::BUFFER_SIZE;

    /*
     * Do the actual read action synchronically
     * Copy data from image file to bp->b_addr
     */
    fseek(this->imgFd, bp->b_blkno*BufferManager::BUFFER_SIZE, SEEK_SET);
    fread(bp->b_addr, sizeof(char), BufferManager::BUFFER_SIZE, imgFd);
    bp->b_flags |= Buf::B_DONE;

    return bp;
}

void BufferManager::Bwrite(Buf *bp)
{
    unsigned int flags;

    flags = bp->b_flags;
    bp->b_flags &= ~(Buf::B_READ | Buf::B_DONE | Buf::B_ERROR | Buf::B_DELWRI);
    bp->b_wcount = BufferManager::BUFFER_SIZE;		/* 512字节 */

    /*
     * Do the actual write action synchronically
     * Copy data from bp->b_addr to the image
     */
    fseek(this->imgFd, bp->b_blkno*512, SEEK_SET);
    fwrite(bp->b_addr, sizeof(char), BufferManager::BUFFER_SIZE, imgFd);
    bp->b_flags |= Buf::B_DONE;

    this->Brelse(bp);
    return;
}

void BufferManager::ClrBuf(Buf *bp)
{
    int* pInt = (int *)bp->b_addr;

    /* 将缓冲区中数据清零 */
    for(unsigned int i = 0; i < BufferManager::BUFFER_SIZE / sizeof(int); i++)
    {
        pInt[i] = 0;
    }
    return;
}

void BufferManager::NotAvail(Buf *bp)
{
    /* 从自由队列中取出 */
    bp->av_back->av_forw = bp->av_forw;
    bp->av_forw->av_back = bp->av_back;
    /* 设置B_BUSY标志 */
    bp->b_flags |= Buf::B_BUSY;
    return;
}


