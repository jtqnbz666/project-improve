#ifndef EVENTDISPATCH_H
#define EVENTDISPATCH_H
#include "CommType.h"
class EventDispatch {
public:
    virtual ~EventDispatch();
    void AddEvent(int fd, uint8_t int_event);
    void RemoveEvent(int fd, uint8_t int_event);
    void StartDispatch(uint32_t wait_timeout = 100);
    void StopDispatch();
    bool isRunning() { return running; }
    static EventDispatch* Instance();
protected:
    EventDispatch();
private:
    int m_epfd;
    static EventDispatch* m_eventDispatchIns;
    bool running;
};
#endif