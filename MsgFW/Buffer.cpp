
#include "Buffer.h"
#include "define.h"

int Buffer::s_nIdCnt = 0;

Buffer::Buffer(int size, int number)
{
    ASSERT(size > 0 && number > 0);

    m_nBufferSize = size * number;
    m_nBitmapSize = (number + 15) / 16;
    m_nSize = size;
    m_nRemain = m_nNumber = number;

    m_pBitmap = new INT16U [m_nBitmapSize];
    m_pBuffer = new BYTE [m_nBufferSize];
    ASSERT(m_pBitmap != NULL && m_pBuffer != NULL);

    for (int i = 0; i < m_nBitmapSize; ++i) {
        m_pBitmap[i] = 0;
    }

    m_nId = ++ s_nIdCnt;
}

Buffer::~Buffer()
{
    delete [] m_pBitmap;
    delete [] m_pBuffer;
}

HBuf Buffer::alloc()
{
    if (m_nRemain == 0)
        return -1;

    /// Enter critical
    for (int i = 0; i < m_nBitmapSize; ++i) {
        INT16U  bits = ~m_pBitmap[i];
        if (bits == 0)
            continue;

        for (   int j = 0; 
                (j < 16) && (i*16+j < m_nNumber);
                ++j, bits >>= 1) {
            if (bits & 1) {
                m_pBitmap[i] |= (1 << j);
                --m_nRemain;
                /// Exit critical
                return (i*16+j) | (m_nId<<24);
            }
        }
    }
    /// Exit critical
    return -1;
}

bool Buffer::free(HBuf hb)
{
    if (!isVaild(hb))
        return false;

    int index = hb & 0x00ffffff;

    /// Enter critical
    m_pBitmap[index / 16] &= ~(0x0001 << (index % 16));
    ++m_nRemain;
    /// Exit critical
    
    return true;
}

bool Buffer::isVaild(HBuf hb)
{
    if (((hb >> 24) == m_nId) && ((hb & 0x00ffffff) < m_nNumber)) {
        return true; 
    }
    return false;
}

void* Buffer::getPtr(HBuf hb)
{
    if (isVaild(hb)) {
        int index = hb & 0x00ffffff; 
        return m_pBuffer + index * m_nSize;
    } else {
        return NULL; 
    }
}

HBuf Buffer::getHandle(const void* pvoid)
{
    if ((pvoid < m_pBuffer) || (pvoid >= (m_pBuffer + m_nNumber * m_nSize)))
        return -1;

    INT32U pos = (INT32U)pvoid - (INT32U)m_pBuffer;
    return (pos / m_nSize) | (m_nId << 24);
}
