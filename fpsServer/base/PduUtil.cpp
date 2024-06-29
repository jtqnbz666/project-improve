#include "PduUtil.h"

string binaryToString(uchar_t* data, size_t size) {
    string result;
    result.reserve(size);
    for (size_t i = 0; i < size; ++i) {
        result += data[i];
    }
    return result;
}

InfiBuffer::InfiBuffer() {
    m_buf = nullptr;
    m_allocSize = 0;
    m_writeOffset = 0;
}

InfiBuffer::~InfiBuffer() {
    m_allocSize = 0;
    m_writeOffset = 0;
    if (m_buf) {
        free(m_buf);
        m_buf = nullptr;
    }
}

void InfiBuffer::Extend(uint32_t len) {
    m_allocSize = m_writeOffset + len;
    m_allocSize += m_allocSize >> 2;
    uchar_t* newBuf = (uchar_t*)realloc(m_buf, m_allocSize);
    m_buf = newBuf;
}