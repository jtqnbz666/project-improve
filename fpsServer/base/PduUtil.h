#ifndef PDUUTIL_H
#define PDUUTIL_H
#include "CommType.h"
string binaryToString(uchar_t* data, size_t size);

// 动态字符串
class InfiBuffer {
public:
    InfiBuffer();
    ~InfiBuffer();
    void Extend(uint32_t len);
    uchar_t* GetBuffer() { return m_buf; }
    uint32_t GetWriteOffset() { return m_writeOffset; }
    uint32_t GetAllocSize() { return m_allocSize; }
    void IncWriteOffset(uint32_t len) { m_writeOffset += len; }

    uint32_t Write(void* buf, uint32_t len);
    void Read(void* buf, uint32_t len);
private:
    uint32_t m_writeOffset;
    uint32_t m_allocSize;
    uchar_t* m_buf;
};

#endif