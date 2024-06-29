#include "BasePdu.h"
#include "BaseConn.h"



InfiPdu::InfiPdu() {

}

InfiPdu::~InfiPdu() {

}

InfiPdu* InfiPdu::ReadPdu(uchar_t* buf, uint32_t len) {
    uint32_t pduLen = 0;
    // if(!IsPduAvailable(buf, len, pduLen)) {
    //     return nullptr;
    // }
    InfiPdu* pdu = NULL;
    pdu = new InfiPdu();
    pdu->Write(buf, len); 
    pdu->ReadPduHeader(buf);

    return pdu;
}

void InfiPdu::SetBodyData(const google::protobuf::Message& msg) {
    m_buf.Read(NULL, m_buf.GetWriteOffset()); 
    m_buf.Write(NULL, sizeof(PduHeader));

    uint32_t msg_size = msg.ByteSize();
    uchar_t* szData = new uchar_t[msg_size];
    if (!msg.SerializeToArray(szData, msg_size)) {
        perror("Failed to serialize protobuf message.");
        delete[] szData;
        return;
    }

    m_buf.Write(szData, msg_size);

    delete[] szData;

    WritePduHeader();
}


uint32_t InfiBuffer::Write(void* buf, uint32_t len) {
	if (m_writeOffset + len > m_allocSize)
	{
		Extend(len);
	}

	if (buf)
	{
		memcpy(m_buf + m_writeOffset, buf, len);
	}

	m_writeOffset += len;

	return len;
}

void InfiBuffer::Read(void* buf, uint32_t len) {
    if(len == 0) {
        return ;
    }
    if(len > m_writeOffset) {
        len = m_writeOffset;
    }

    if(buf) {
        memcpy(buf, m_buf, len);
    }

    m_writeOffset -= len;
    memmove(m_buf, m_buf + len, m_writeOffset);
}


void InfiPdu::ReadPduHeader (uchar_t* buf) {
    uint32_t pduLen = ByteStream::ReadUint32(buf); //第一个字节就是长度
    uint16_t commandID = ByteStream::ReadUint16(buf + 4);
    uint16_t seqNum = ByteStream::ReadUint16(buf + 6);

    printf("ReadPduHeader:%d,%d,%d\n", pduLen, commandID, seqNum);
    m_header.length = pduLen;
    m_header.commandID = commandID;
    m_header.seqNum = seqNum;
}

void InfiPdu::WritePduHeader () {
    PduHeader header;
    header.length = htonl(m_buf.GetWriteOffset());
    header.commandID = htons(GetCommandID());
    header.seqNum = htons(GetSeqNum());

    // Copy the header to the buffer
    memcpy(GetBuffer(), &header, sizeof(header));
    printf("WritePduHeader:%d,%d,%d\n",header.length, header.commandID, header.seqNum);
}

bool InfiPdu::IsPduAvailable(uchar_t* buf, uint32_t len, uint32_t& pduLen) {
    if(len < IM_PDU_HEADER_LEN) {
        return false;
    }
    pduLen = ByteStream::ReadUint32(buf); //第一个字节就是长度
    printf("pduLen:%d, practicalLen:%d\n", pduLen, len);
    if(pduLen > len) {
        return false;
    }
    if(0 == pduLen) {
        perror("IsPduAvailable failed");
        exit(0);
    }
    return true;
}

uint32_t ByteStream::ReadUint32(uchar_t* buf) {
    uint32_t data = (buf[0] << 24) | (buf[1] << 16) | (buf[2] << 8) | buf[3];
    return data;
}

uint16_t ByteStream::ReadUint16(uchar_t* buf) {
    uint16_t data = (buf[0] << 8) | buf[1];
    return data;
}
