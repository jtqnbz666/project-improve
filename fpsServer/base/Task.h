#ifndef TASK_H
#define TASK_H
#include "CommType.h"
#include "BasePdu.h"

class InfiPdu;

class CTask {
public:
    CTask();
    ~CTask();
    void SetPdu(InfiPdu* pdu);
    virtual void Run();

private:
    InfiPdu* m_pdu;
};

#endif