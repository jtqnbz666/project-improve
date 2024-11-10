// StatsUtils.hpp
#ifndef STATS_UTILS_HPP
#define STATS_UTILS_HPP

#include <string>
#include "../ConnService.h"
#include "../ProtocolDef.h"

class StatsUtils {
public:
    // 根据 msgID 获取统计键
    static std::string GetStatKeyFromMsgID(int msgID);
};

#endif // STATS_UTILS_HPP
