#include "util.hpp"

string StatsUtils::GetStatKeyFromMsgID(int msgID) {
    string baseKey = "AuthServer.req.";
    string suffix = ".count";

   // 消息类型到统计键的映射规则
    switch (msgID) {
        case MSGID_REGISTER:
            return baseKey + "REGISTER_REQ" + suffix;
            break;
        case MSGID_PWD_LOGIN:
            return baseKey + "PWD_LOGIN_REQ" + suffix;
            break;
        case MSGID_TOKEN_LOGIN:
            return baseKey + "TOKEN_LOGIN_REQ" + suffix;
            break;
        case (MESSAGE_TYPE_SEND_VERIFY_CODE | MESSAGE_TYPE_REQ):
            return baseKey + "VERIFY_CODE_REQ" + suffix;
            break;
        case (MESSAGE_TYPE_PHONE_NUMBER_LOGIN | MESSAGE_TYPE_REQ):
            return baseKey + "PHONE_NUMBER_LOGIN_REQ" + suffix;
            break;
        case (MESSAGE_TYPE_CHANGE_NAME | MESSAGE_TYPE_REQ):
            return baseKey + "CHANGE_NAME_REQ" + suffix;
            break;
        default:
            return baseKey + "OTHER";
    }
}
