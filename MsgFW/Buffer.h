#ifndef __BUFFER_H__
#define __BUFFER_H__

#include "type.h"

typedef signed int  HBuf;

/**
 * Buffer class define
 * \brief Buffer can apply fix size buffer oalloc operation.\n
 *        and if we don't use it, we can call free() to release it.
 */
class Buffer
{
public:
    Buffer(int size, int number);
    ~Buffer();

    HBuf alloc();
    bool free(HBuf hb);

    void* getPtr(HBuf hb);
    HBuf getHandle(const void* ptr);

    int getSize();
    int getNumber();
    int getRemain();
    bool isVaild(HBuf hb);

private:
    int m_nBitmapSize;
    int m_nBufferSize;
    int m_nNumber;  /**< Buffer's max number */
    int m_nSize;    /**< Buffer's fixed size */
    int m_nRemain;

    INT16U *m_pBitmap;  /**< Bitmap to record which is used and unused. */
    BYTE   *m_pBuffer;  /**< Buffer to be alloced. */

    int     m_nId;
    static int s_nIdCnt;
};

#endif  //__BUFFER_H__
