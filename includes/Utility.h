#ifndef UITILITY_H
#define UITILITY_H


/*
 *@comment 一些经常被使用到的工具函数
 *
 *
 */
class Utility
{
    public:
        static void MemCopy(unsigned long src, unsigned long des, unsigned int count);

        static int CaluPageNeed(unsigned int memoryneed, unsigned int pagesize);

        static void StringCopy(char* src, char* dst);

        static int StringLength(char* pString);

        /* 以src为源地址，dst为目的地址，复制count个双字 */
        static void DWordCopy(int* src, int* dst, int count);

        static int Min(int a, int b);

        static int Max(int a, int b);

        /* Convert BCD to Binary */
        static int BCDToBinary(int value);

        /* 用于在读、写文件时，高速缓存与用户指定目标内存区域之间数据传送 */
        static void IOMove(unsigned char* from, unsigned char* to, int count);
};

#endif
