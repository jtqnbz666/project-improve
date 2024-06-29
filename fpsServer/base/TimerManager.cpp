#include "TimerManager.h"

int64_t TimerManager::gid = 0;

time_t TimerManager::GetTick() {  //
    auto sc = chrono::time_point_cast<chrono::milliseconds>(chrono::steady_clock::now());
    auto tmp = chrono::duration_cast<chrono::milliseconds>(sc.time_since_epoch());//duration 持续时间  ，， epoch时代
    return tmp.count();
}

TimerNode TimerManager::AddTimer(time_t msec, Callback func) {
    time_t expire = GetTick()+msec;
    auto ele = timer.emplace(expire, GenID(), func);
    return static_cast<TimerNode>(*ele.first);
}

bool TimerManager::DelTimer(TimerNode &node) {
    auto iter = timer.find(node); // 能不能通过 TimerNode 找到 timernode？ C++14 才可以 可以通过等价key
    if (iter != timer.end()) {
        timer.erase(iter);
        return true;
    }
    return false;
}

bool TimerManager::CheckTimer() {
    auto iter = timer.begin();
    if (iter != timer.end() && iter->expire <= GetTick()) {
        iter->func();
        timer.erase(iter);
        return true;
    }
    return false;
}
time_t TimerManager::TimeToSleep() {
    auto iter = timer.begin();
    if (iter == timer.end()) return -1;
    time_t diss = iter->expire - GetTick();
    return diss > 0 ? diss : 0;
}