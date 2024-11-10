# -*- coding: utf-8 -*-
MESSAGE_TYPE_DICT = {
    "REQ": (0, "请求"),
    "ACK": (100000000, "响应"),
    
    "PHONE_NUMBER_LOGIN": (100, "手机号登陆"),
    "SEND_VERIFY_CODE": (101, "发送验证码"),

    "JOIN_TABLE": (1000, "请求加入游戏桌"),
    "CREATE_TABLE": (1001, "请求创建游戏桌"),
    "READY_GAME": (1002, "准备游戏"),
    "START_MATCH": (1003, "请求开始匹配"),
    "DOWN_CHESS": (1004, "请求下棋"),
    "CANCEL_MATCH": (1005, "通知取消匹配"),
    "START_GAME": (1015, "请求开始游戏"),

    # 通知类
    "UPDATE_TABLE_STATUS": (2001, "更新游戏桌状态"),
    "NOTIFY_END_GAME": (2002, "通知结束游戏"),
    "NOTIFY_START_GAME": (2003, "通知开始游戏"),
    "NOTIFY_START_LOCAL_ROOM_GAME": (2004, "通知局域网对局开始"),

    #局内游戏
    "DRAW_CHESS":(3000, "下棋"),
    "MY_DRAW_CHESS":(3001, "通知下棋"),
    "GAME_OVER":(3002,"游戏结束"),
    "REGRET_CHESS":(3003,"悔棋"),
    "ASK_REGRET_CHESS":(3004,"通知悔棋"),
    "AGREE_REGRET_CHESS":(3005,"同意悔棋"),
    "NOTIFY_AGREE_REGRET_CHESS":(3006,"通知同意悔棋"),
    "DISAGREE_REGRET_CHESS":(3007,"不同意悔棋"),

    "NOTIFY_DISAGREE_REGRET_CHESS":(3008,"不同意悔棋"),
   
    # 玩家数据变更
    "CHANGE_NAME": (4001, "修改昵称"),

    # 局域网对局
    "LOCAL_ROOM_DRAW_CHESS": (5001, "局域网下棋"),

    "NOTIFY_DISAGREE_REGRET_CHESS":(3008,"通知不同意悔棋"),
    "NOTIFY_GIVE_UP":(3009,"认输"),
    "NOTIFY_PEACE_CHESS":(3010,"请求和棋"),
    "NOTIFY_ASK_PEACE_CHESS":(3011,"通知和棋请求"),
    "NOTIFY_AGREE_PEACE_CHESS":(3012,"同意和棋请求"),
    "NOTIFY_DISAGREE_PEACE_CHESS":(3013,"不同意和棋请求"),
    "NOTIFY_ASK_AGREE_PEACE_CHESS":(3014,"通知同意和棋请求"),
    "NOTIFY_ASK_DISAGREE_PEACE_CHESS":(3015,"通知不同意和棋请求"),

    "NOTIFY_GAME_SETTLEMENT":(3016,"对局结算"),

    # 排行榜
    "REQ_LEADER_BOARD_INFO": (6001, "请求排行榜数据"),
}

DO_NOT_EDIT = "// Code generated by tool. DO NOT EDIT."

def gen_msg_type_name(name):
    return f"MESSAGE_TYPE_{name}"

def generate_message_type_header():
    code = []

    code.append(DO_NOT_EDIT)
    code.append("#ifndef MESSAGE_TYPE_H")
    code.append("#define MESSAGE_TYPE_H")
    code.append("")

    for name, (code_val, comment) in MESSAGE_TYPE_DICT.items():
        code.append(f"const int {gen_msg_type_name(name)} = {code_val}; // {comment}")

    code.append("")
    code.append("#endif // MESSAGE_TYPE_H")

    save("MessageType.h", code)

def save(filename, code):
    with open(filename, "w") as f:
        f.write("\n".join(code))
        print(f"{filename} has been generated.")

if __name__ == "__main__":
    generate_message_type_header()