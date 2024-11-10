#ifndef _GAMEINFO_SERVICE_H_
#define _GAMEINFO_SERVICE_H_

#include "service.h"
#include "protocol.h"
#include "ProtocolDef.h"
#include "RedisIntf.h"
#include "PlayerMgr.h"
#include "AppDef.h"
#include <set>
#include "wzqProtocol.pb.h"
#include "wzqGame.pb.h"

using google::protobuf::int32;
using google::protobuf::int64;
class CConnService : public CService
{
    friend class CLogSvrd ;
public:
	CConnService();
	virtual ~CConnService();
public:
	
	virtual IEventPump *mcgclient(DWORD dwIP,WORD wPort);
	virtual bool OnInit();
	virtual bool OnMsg(DWORD dwMsgID, DWORD dwSeqID);
	virtual bool OnTimeOut(DWORD dwEventID);
	virtual void OnExitProcess(UINT uExitCode);
    virtual bool OnReceiveQueueMsg(BYTE* pbyBuffer, int nBufferLen);
    virtual bool OnAsyncMsg(DWORD dwMsgID, DWORD dwSeqID, int nErrorCode, void* pvContext){return true;}
    
public:
    void OnACKRegister(MsgPackage msg);
    void OnACKPWDLogin(MsgPackage msg);
    void OnACKTokenLogin(MsgPackage msg);
    void OnACKPhoneNumberLogin(MsgPackage msg);
    void OnACKSendVerifyCode(MsgPackage msg);
    void OnACKCreateTable(MsgPackage msg);
    void OnACKJoinTable(MsgPackage msg);
    void OnACKStartGame(MsgPackage msg);
    void OnACKReadyGame(MsgPackage msg);
    void OnUpdateTableStatus(MsgPackage msg);
    void OnACKMatch(MsgPackage msg);
    void OnACKChangeName(MsgPackage msg);
    void OnNotifyStartGame(MsgPackage msg);
    void OnACKDrawChess(MsgPackage msg);
    void OnGameOver(MsgPackage msg);
    void OnACKRegretChess(MsgPackage msg);
    void OnACKAgreeRegretChess(MsgPackage msg);
    void OnACKDisagreeRegretChess(MsgPackage msg);
    void OnNotifyASKPeaceChess(MsgPackage msg);
    void OnNotifyASKDisagreePeaceChess(MsgPackage msg);
    void OnACKLeaderBoard(MsgPackage msg);

public :
    void OnSvrdClose();
    bool SvrdNeedClose();
    //获取该连接服务器的路由消息数据
    std::string GetConnSvrdMsgRoute();
    //获取鉴权服务路由信息
    std::string GetAuthSvrdMsgRoute();
    //获取game服务路由信息
    std::string GetGameSvrdMsgRoute();
    //获取game服务路由信息
    std::string GetRoomSvrdMsgRoute();
    //向内部服务转发消息
    int SendSvrdMsg(const std::string& msgroute , int64 msg_id , int64 seq_id , int64 user_id , const std::string& msg,int64 nClientID);
    //向客户端转发消息
    int SendClientMsg(int64 userid , DWORD dwMsgID, const std::string& strBuffer, DWORD dwSeqID , int64 nClientID);
private:
    bool InitRedis();
    
    bool InitRecvMsgQueueThread();
    
    static void* RecvProc(void* pParam);
    //初始化消息队列缓存
    bool InitMQRedis();
public:
	CRedisIntf*                         m_pRedis;
    CPlayerMgr*                         m_pPlayerMgr;
    bool                                m_bSvrdClose        ;  //服务器收到关闭的指令 ,即将关闭
};

#endif 

