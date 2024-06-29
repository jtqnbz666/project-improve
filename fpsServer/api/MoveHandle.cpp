#include "MoveHandle.h"
#include "RouterFactory.h"

void MoveHandle::Handle(InfiPdu* pdu) {
    printf("MoveHandle\n");
}

// 在编译器就可以注册路由了，nb
REGISTER_HANDLE("1", MoveHandle);
