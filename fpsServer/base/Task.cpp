#include "Task.h"
#include "Router.h"
#include "RouterFactory.h"
CTask::CTask() {
    m_pdu = nullptr;
}

CTask::~CTask() {

}

void CTask::SetPdu(InfiPdu* pdu) {
    m_pdu = pdu;
}

void CTask::Run() {
    printf("执行了CTask::Run\n");
    if(m_pdu != nullptr) {
        // 根据commandID拿到handle
        Router* routerHandle = RouterFactory::instance().createRouter("1");
        if (routerHandle != nullptr) {
            printf("exist routerHandle\n");
        }
        routerHandle->Handle(m_pdu);
    }
}