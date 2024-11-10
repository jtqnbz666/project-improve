#include "util.hpp"

string StatsUtils::GetStatKeyFromMsgID(int msgID) {
    string baseKey = "ConnServer.req.";
    string suffix = ".count";

    // 消息类型到统计键的映射规则
    switch (msgID) {
        case (MSGID_REGISTER | ID_ACK):
            return baseKey + "REGISTER_ACK" + suffix;
            break;
        case (MSGID_PWD_LOGIN | ID_ACK):
            return baseKey + "PWD_LOGIN_ACK" + suffix;
            break;
        case (MSGID_TOKEN_LOGIN | ID_ACK):
            return baseKey + "TOKEN_LOGIN_ACK" + suffix;
            break;
        case (MESSAGE_TYPE_CREATE_TABLE | MESSAGE_TYPE_ACK):
            return baseKey + "CREATE_TABLE_ACK" + suffix;
            break;
        case (MESSAGE_TYPE_JOIN_TABLE | MESSAGE_TYPE_ACK):
            return baseKey + "JOIN_TABLE_ACK" + suffix;
            break;
        case (MESSAGE_TYPE_START_GAME | MESSAGE_TYPE_ACK):
            return baseKey + "START_GAME_ACK" + suffix;
            break;
        case (MESSAGE_TYPE_READY_GAME | MESSAGE_TYPE_ACK):
            return baseKey + "START_GAME_ACK" + suffix;
            break;
        case MESSAGE_TYPE_UPDATE_TABLE_STATUS:
            return baseKey + "UPDATE_TABLE_STATUS" + suffix;
            break;
        case (MESSAGE_TYPE_SEND_VERIFY_CODE | MESSAGE_TYPE_ACK):
            return baseKey + "SEND_VERIFY_CODE_ACK" + suffix;
            break;
        case (MESSAGE_TYPE_PHONE_NUMBER_LOGIN | MESSAGE_TYPE_ACK):
            return baseKey + "PHONE_NUMBER_LOGIN" + suffix;
            break;
        case (MSGID_REGISTER | ID_REQ):
            return baseKey + "REGISTER_REQ" + suffix;
            break;
        case (MSGID_PWD_LOGIN | ID_REQ):
            return baseKey + "PWD_LOGIN_REQ" + suffix;
            break;
        case (MSGID_TOKEN_LOGIN | ID_REQ):
            return baseKey + "TOKEN_LOGIN_REQ" + suffix;
            break;
        case (MESSAGE_TYPE_PHONE_NUMBER_LOGIN | MESSAGE_TYPE_REQ):
            return baseKey + "PHONE_NUMBER_LOGIN_REQ" + suffix;
            break;
        case (MESSAGE_TYPE_SEND_VERIFY_CODE | MESSAGE_TYPE_REQ):
            return baseKey + "SEND_VERIFY_CODE_REQ" + suffix;
            break;
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
        case (MESSAGE_TYPE_CHANGE_NAME | MESSAGE_TYPE_ACK):
            return baseKey + "CHANGE_NAME_ACK" + suffix;
            break;
        case MESSAGE_TYPE_CHANGE_NAME | MESSAGE_TYPE_REQ:
		    return baseKey + "CHANGE_NAME_REQ" + suffix;
            break;
        default:
            return baseKey + "OTHER" + suffix;
    }
}
