#include "util.hpp"

// 生成统计键的函数
std::string StatsUtils::GetStatKeyFromMsgID(int msgID) {
    string baseKey = "RoomServer.req.";
    string suffix = ".count";

    switch (msgID) {
        case MESSAGE_TYPE_CREATE_TABLE | MESSAGE_TYPE_REQ:
            return baseKey + "CREATE_TABLE_REQ" + suffix;
			break;
        case MESSAGE_TYPE_JOIN_TABLE | MESSAGE_TYPE_REQ:
            return baseKey + "JOIN_TABLE_REQ" + suffix;
			break;
        case MESSAGE_TYPE_READY_GAME | MESSAGE_TYPE_REQ:
            return baseKey + "READY_GAME_REQ" + suffix;
			break;
        case MESSAGE_TYPE_START_GAME | MESSAGE_TYPE_REQ:
            return baseKey + "START_GAME_REQ" + suffix;
			break;
        
        default:
            return baseKey + "OTHER";
    }
}
