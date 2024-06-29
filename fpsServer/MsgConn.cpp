#include "MsgConn.h"

void MsgConn::ChangeConnMap(int fd) {
    globalSocketMap[fd] = this;
}

void MsgConn::InitMsgConn(int fd) {
    BaseConn* bConn = globalSocketMap[fd];
    SetSocket(fd);
    SetCallback(BaseConnCallback);
    SetLocalIP(bConn->GetLocalIP());
    SetLocalPort(bConn->GetLocalPort());
    SetTcpMode(bConn->IsTcpMode());
    SetRemoteIP(bConn->GetRemoteIP());
    SetRemotePort(bConn->GetRemotePort());

    ChangeConnMap(fd);
    free(bConn);
}

void MsgConn::HandlePdu(InfiPdu* pdu) {
    //
    printf("调用了HandlePdu, GetCommandID:%d, bodyLenth:%d\n", pdu->GetCommandID(), pdu->GetBodyLength());
    switch(pdu->GetCommandID()) {
        case 1:
        printf("GetCommandID为1\n");
        break;
        default:
        printf("no handle method\n");
    }
}