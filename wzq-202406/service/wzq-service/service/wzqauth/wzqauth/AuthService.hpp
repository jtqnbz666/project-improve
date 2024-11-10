//
//  AuthService.hpp
//  authservice
//
//  Created by 刘挺 on 2022/11/24.
//

#ifndef AuthService_hpp
#define AuthService_hpp
#define IN
#define OUT
#define INOUT
#define MAX_SQL_LEN	4*1024
#define SQL_SUCCESS 0
#define SQL_FAILED  -1
#include "service.h"
#include "sampledbpool.h"
#include <mysql/mysql.h>

#include <string>
#include <google/protobuf/stubs/common.h>
#include "AppDef.h"
#include "SecurityTool.h"
#include "proto/wzqProtocol.pb.h"
#include "proto/wzqGame.pb.h"
#include <random> 
#include <json/json.h>
#include "LeaderBoard.hpp"

class CSampleDBPool;

class CAuthService : public CService
{
public:
    CAuthService();
    virtual ~CAuthService();

public:
        int AuthConnect(const string &strHost, 
            const string &strUser, 
            const string &strPwd, 
            const string &strDBName, 
            unsigned int nPort, 
            const string &strCharSet="utf8");
        int AuthClose();

    int AuthPing();

    int AuthCommit();

    int AuthRollback();
    virtual IEventPump *mcgclient(DWORD dwIP,WORD wPort){return nullptr;}
    
    virtual bool OnInit();

    virtual bool OnMsg(DWORD dwMsgID, DWORD dwSeqID){return true;}

    virtual bool OnTimeOut(DWORD dwEventID){return true;}

    virtual void OnExitProcess(UINT uExitCode){return;}
    
    virtual bool OnReceiveQueueMsg(BYTE* pbyBuffer, int nBufferLen);
    
    virtual bool OnAsyncMsg(DWORD dwMsgID, DWORD dwSeqID, int nErrorCode, void* pvContext){return true;}
    

public:
    void OnREQRegister(MsgPackage& msg);
    void OnREQPWDLogin(MsgPackage& msg);
    void OnREQTokenLogin(MsgPackage& msg);
    void OnREQPhoneNumberLogin(MsgPackage& msg);
    void OnREQSendVerifyCode(MsgPackage& msg);
    bool VerifyCode(const std::string& phoneNumber, const std::string& code);
    void OnREQChangeName(MsgPackage& msg);
    void OnNotifyGameSettlement(MsgPackage& msg);
    void OnREQLeaderBoardInfo(MsgPackage& msg);
    void SetBaseInfo(UserRawData& baseInfo);
    UserRawData GetBaseInfo(int64 userID);
    string GenUserRawDataKey(int64 userID);
    string GetStatKeyFromMsgID(int msgID);
public :
    void OnSvrdClose(){m_bSvrdClose = true;}
    bool SvrdNeedClose(){return m_bSvrdClose;}
    //获取该服务器的路由配置
    std::string GetAuthSvrdMsgRoute();
    //获取连接服的路由信息
    std::string GetConnSvrdMsgRoute();
    //获取gm服的路由信息
    std::string GetGMToolRoute();
    //收取消息线程
    bool InitRecvMsgQueueThread();
    static void* RecvProc(void* pParam);
    //初始化数据库服务
    bool InitDBService();
    //初始化Redis
    bool InitRedisService();
    //向内部服务转发消息
    int SendSvrdMsg(const std::string& msgroute , int64 msg_id , int64 seq_id , int64 user_id , const std::string& msg,int64 nClientID);
    std::string GenUserDisplayId(int length);
    redisContext* GetRedisIns();
    
public:
    bool                                m_bSvrdClose        ;  //服务器收到关闭的指令 ,即将关闭
    CSampleDBPool*                      m_pSampleDBPool     ;  //数据库
private:
    MYSQL *m_hMySQL;
    redisContext* m_pRedisIns;
	char m_szSQL[MAX_SQL_LEN];
	bool m_bPrint;
};

#endif /* AuthService_hpp */
