#include "TimerNode.h"

TimerNode::TimerNode(time_t exp, int64_t id, Callback func) : func(func) {
    this->expire = exp;
    this->id = id;
}