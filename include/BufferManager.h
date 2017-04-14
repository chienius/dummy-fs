#ifndef BUFFERMANAGER_H
#define BUFFERMANAGER_H
#include <cstdio>
#include "Buf.h"

class BufferManager
{
    public:
        /* static const member */
        static const int NBUF = 15;			/* 缓存控制块、缓冲区的数量 */
        static const int BUFFER_SIZE = 512;	/* 缓冲区大小。 以字节为单位 */

    public:
        void Initialize(FILE* fd);					/* 缓存控制块队列的初始化。将缓存控制块中b_addr指向相应缓冲区首地址。*/

        Buf* GetBlk(short dev, int blkno);	/* 申请一块缓存，用于读写设备dev上的字符块blkno。*/
        void Brelse(Buf* bp);				/* 释放缓存控制块buf */
        Buf* Bread(short dev, int blkno);	/* 读一个磁盘块。dev为主、次设备号，blkno为目标磁盘块逻辑块号。 */
        void Bwrite(Buf* bp);				/* 写一个磁盘块 */

        void ClrBuf(Buf* bp);				/* 清空缓冲区内容 */
        void Bflush(short dev);				/* 将dev指定设备队列中延迟写的缓存全部输出到磁盘 */
        Buf& GetBFreeList();				/* 获取自由缓存队列控制块Buf对象引用 */

        Devtab devtab;
        FILE*  imgFd;

    private:
        void NotAvail(Buf* bp);				/* 从自由队列中摘下指定的缓存控制块buf */

    private:
        Buf bFreeList;						/* 自由缓存队列控制块 */
        Buf SwBuf;							/* 进程图像传送请求块 */
        Buf m_Buf[NBUF];					/* 缓存控制块数组 */
        unsigned char Buffer[NBUF][BUFFER_SIZE];	/* 缓冲区数组 */
};

extern BufferManager g_BufferManager;
#endif /* ifndef BUFFERMANAGER_H */
