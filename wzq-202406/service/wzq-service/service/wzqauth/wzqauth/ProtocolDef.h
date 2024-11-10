#ifndef __PROTOCOL_DEF_H__
#define __PROTOCOL_DEF_H__

//	消息基础值定义
#define MSGID_BASE_SVRD_INSIDE  0x00000400      // 服务内部通信
#define	MSGID_BASE_GS_GAMEINFO	0x00000500		// gamesvrd和gameinfo交互
#define	MSGID_BASE_GS_WEB		0x00000600		// gamesvrd和web交互
#define	MSGID_BASE_GS_MANAGER	0x00000700		// gamesvrd和manager交互
#define MSGID_BASE_GS_AI		0x00000800		// gamesvrd和ai交互
#define MSGID_BASE_GS_RM        0x00000900      // 同roomsvrd 交互
#define	MSGID_BASE_GS_CLIENT	0x00010000		// gamesvrd和client交互

//client
#define  MSGID_HOLD					(MSGID_BASE_GS_CLIENT + 0)		//保持连接
#define  MSGID_REGISTER             (MSGID_BASE_GS_CLIENT + 1)      //注册
#define  MSGID_PWD_LOGIN            (MSGID_BASE_GS_CLIENT + 2)      //密码登录
#define  MSGID_TOKEN_LOGIN          (MSGID_BASE_GS_CLIENT + 3)      //token登录


enum ENUM_REGISTER_RESULT
{
    register_result_success = 200 ,
    register_result_failed_unknown = 201 , //位置错误
    register_result_failed_exists   = 202 ,//用户名已存在
};
enum ENUM_LOGIN_RESULT
{
    login_result_success = 200 ,
    login_result_failed_unknown = 201,  //未知错误
    login_result_failed_passwd_error= 202,  //密码错误
    login_result_failed_token_error= 203,  //token有误
};
#endif



