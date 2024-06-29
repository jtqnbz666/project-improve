#ifndef MOVEHANDLE_H
#define MOVEHANDLE_H
#include "RouterFactory.h"
#include "BasePdu.h"

class MoveHandle : public Router {
public:
    virtual void Handle(InfiPdu* pdu);
};

#endif