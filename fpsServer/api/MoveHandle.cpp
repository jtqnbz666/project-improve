#include "MoveHandle.h"
#include "RouterFactory.h"

void MoveHandle::Handle(InfiPdu* pdu) {
    printf("MoveHandle\n");

     // 初始化 Protocol Buffers 库
    GOOGLE_PROTOBUF_VERIFY_VERSION;

    // 创建 Position 对象
    Position testPos;
    testPos.set_x(1.0);
    testPos.set_y(2.0);
    testPos.set_z(3.0);
    testPos.set_yaw(4.0);
    testPos.set_pitch(5.0);
    testPos.set_roll(6.0);
    
    InfiPdu *backPdu = new InfiPdu();
    backPdu->SetCommandID(1);
    backPdu->SetSeqNum(666);
    pdu->SetBodyData(testPos);
    backPdu->GetBaseConn()->SendPdu(backPdu);
    // 
}

// 在编译器就可以注册路由了，nb
REGISTER_HANDLE("1", MoveHandle);
