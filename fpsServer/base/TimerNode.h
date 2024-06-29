#ifndef TIMERNODE_H
#define TIMERNODE_H
#include "CommType.h"

class TimerNode {
public:
    // C++ 11特性，使用函数对象。降低拷贝消耗，提高效率
    TimerNode(time_t exp, int64_t id, Callback func);

    Callback func;
    time_t expire;  // 过期时间
    int64_t id;     // 全局唯一id
};

#endif