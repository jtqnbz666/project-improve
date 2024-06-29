#ifndef ROUTER_H
#define ROUTER_H
#include "BasePdu.h"
class InfiPdu;
class Router {
public:
    virtual void Handle(InfiPdu* pdu) = 0;
};

#endif