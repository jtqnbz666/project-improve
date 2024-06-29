#ifndef BasePdu_H
#define BasePdu_H
#include "CommType.h"
#include "PduUtil.h"

#define IM_PDU_HEADER_LEN           6

// 前向声明
class BaseConn;

typedef struct {
    uint32_t length;
    uint16_t commandID;
    uint16_t seqNum;
    
} PduHeader;

// 字节流工具
class ByteStream {
public:
    static uint32_t ReadUint32(uchar_t* buf);
    static uint16_t ReadUint16(uchar_t* buf);
};


class InfiPdu {
public:
    InfiPdu();
    ~InfiPdu();
    
    static bool IsPduAvailable(uchar_t* buf, uint32_t len, uint32_t& pduLen);
    static InfiPdu* ReadPdu(uchar_t* buf, uint32_t len);
    void ReadPduHeader(uchar_t* buf);
    void Write(uchar_t* buf, uint32_t len) { m_buf.Write((void*)buf, len); }
    void SetBaseConn(BaseConn* baseConn) { m_conn = baseConn;}
    void WritePduHeader();

    uchar_t* GetBuffer() { return m_buf.GetBuffer(); }
    // 总长
    uint32_t GetBufferLength() { return m_buf.GetWriteOffset(); }
    // 消息体长度
    uint32_t GetBodyLength() { return m_buf.GetWriteOffset() - sizeof(PduHeader); }
    // 头部字段的长度
    uint32_t GetPduLength() { return m_header.length; }
    
    uint16_t GetCommandID() { return m_header.commandID; }
    uint16_t GetSeqNum() { return m_header.seqNum; }
    BaseConn* GetBaseConn() { return m_conn; }
    uchar_t* GetBodyData() { return m_buf.GetBuffer() + sizeof(PduHeader); }

    void SetBodyData(const google::protobuf::Message& msg);
    void SetCommandID(uint16_t id) { m_header.commandID = id; }
    void SetSeqNum(uint16_t seqNum) { m_header.seqNum = seqNum; }
    void SetPduLength(uint32_t len) { m_header.length = len; }
    

private:
    InfiBuffer m_buf;
    PduHeader m_header;
    BaseConn* m_conn;
};


#endif