#ifndef MOVEHANDLE_H
#define MOVEHANDLE_H
#include "RouterFactory.h"
#include "BasePdu.h"
#include "BaseConn.h"
#include "message.pb.h"
#include "ConnManager.h"
class MoveHandle : public Router {
public:
    virtual void Handle(InfiPdu* pdu);
};

#endif