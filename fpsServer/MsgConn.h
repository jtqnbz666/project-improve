#include "BaseConn.h"
#include "BasePdu.h"
class MsgConn : public BaseConn {
public:
    void InitMsgConn(int fd);
    void ChangeConnMap(int fd);
    virtual void HandlePdu(InfiPdu* pdu);
};