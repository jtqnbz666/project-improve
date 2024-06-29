#include "MoveHandle.h"
#include "RouterFactory.h"

void MoveHandle::Handle(InfiPdu* pdu) {
    printf("MoveHandle\n");


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
    backPdu->SetBodyData(testPos);
    pdu->GetBaseConn()->SendPdu(backPdu);

    
    //测试readPdu
    Position msg;
    msg.ParseFromArray(backPdu->GetBodyData(), backPdu->GetBodyLength());
    printf("%.1f:%.1f:%.1f:%.1f:%.1f\n", msg.x(), msg.y(), msg.z(), msg.yaw(), msg.pitch());

    // 转发给所有玩家此位置信息
    ConnManager::Instance().BroadcastMessageExceptSender(pdu->GetBaseConn()->GetSocket() ,backPdu);
}

// 在编译器就可以注册路由了，nb
REGISTER_HANDLE("1", MoveHandle);
