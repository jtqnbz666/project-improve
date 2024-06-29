#ifndef TIMERMANAGER_H
#define TIMERMANAGER_H
#include "CommType.h"
#include "TimerNode.h"
#include <chrono>
#include <memory>
#include <set>
// 引用依然有 
struct TimerNodeCompare {
    bool operator()(const TimerNode& lhd, const TimerNode& rhd) const {
        if (lhd.expire < rhd.expire)
            return true;
        else if (lhd.expire > rhd.expire)
            return false;
        return lhd.id < rhd.id;
    }
};

class TimerManager {
public:
    static time_t GetTick();
    TimerNode AddTimer(time_t msec, Callback func);
    bool DelTimer(TimerNode &node);
    bool CheckTimer();
    time_t TimeToSleep();
    
private:
    static int64_t GenID() {
        return gid++;
    }
    set<TimerNode, TimerNodeCompare> timer;
    static int64_t gid;
};

#endif