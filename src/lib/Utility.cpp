#include "DummyKernel.h"
#include "Utility.h"
#include "User.h"

void Utility::MemCopy(unsigned long src, unsigned long des, unsigned int count)
{
    unsigned char* psrc = (unsigned char*)src;
    unsigned char* pdes = (unsigned char*)des;

    for ( unsigned int i = 0; i < count; i++ ) 
        pdes[i] = psrc[i];
}

int Utility::CaluPageNeed(unsigned int memoryneed, unsigned int pagesize)
{
    int pageRequired = memoryneed / pagesize;
    pageRequired += memoryneed % pagesize ? 1 : 0;

    return pageRequired;
}

void Utility::StringCopy(char* src, char* dst)
{
    while ( (*dst++ = *src++) != 0 ) ;
}

int Utility::StringLength(char* pString)
{
    int length = 0;
    char* pChar = pString;

    while( *pChar++ )
    {
        length++;
    }

    /* 返回字符串长度 */
    return length;
}


void Utility::DWordCopy(int *src, int *dst, int count)
{
    while(count--)
    {
        *dst++ = *src++;
    }
    return;
}

int Utility::Min(int a, int b)
{
    if(a < b)
        return a;
    return b;
}

int Utility::Max(int a, int b)
{
    if(a > b)
        return a;
    return b;
}

int Utility::BCDToBinary( int value )
{
    return ( (value >> 4) * 10 + (value & 0xF) );
}

void Utility::IOMove(unsigned char* from, unsigned char* to, int count)
{
    while(count--)
    {
        *to++ = *from++;
    }
    return;
}

