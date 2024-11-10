#ifndef _GAMEINFO_SERVICE_H_
#define _GAMEINFO_SERVICE_H_

#include "service.h"
#include "protocol.h"
#include "ProtocolDef.h"
#include "RedisIntf.h"
#include "AppDef.h"
#include <set>
#include "wzqProtocol.pb.h"
#include "wzqGame.pb.h"
#include "MatchQueue.hpp"

using google::protobuf::int32;
using google::protobuf::int64;

using namespace wzq;

// 匹配频率5s
#define MATCH_FREQUENCE 5000

class CRoomService : public CService
{
    friend class CLogSvrd ;
public:
	CRoomService();
	virtual ~CRoomService();

public:
	virtual IEventPump *mcgclient(DWORD dwIP,WORD wPort);

	virtual bool OnInit();

	virtual bool OnTimeOut(DWORD dwEventID);

	virtual void OnExitProcess(UINT uExitCode);
    
    virtual bool OnReceiveQueueMsg(BYTE* pbyBuffer, int nBufferLen);
    
    //向内部服务转发消息
    int SendSvrdMsg(const std::string& msgroute , int64 msg_id , int64 seq_id , int64 user_id , const std::string& msg,int64 nClientID);
    
    static LPTHREAD_START_ROUTINE PMatch(LPVOID arg) {((CRoomService*)arg)->OnMatch();return 0;}
    DWORD OnMatch();
    
public :
    void OnSvrdClose();
    bool SvrdNeedClose();
    std::string GetRoomSvrdMsgRoute();
    std::string GetConnSvrdMsgRoute();
    std::string GetGameSvrdMsgRoute();
    
public:
    void OnREQMatchGame(MsgPackage msg);
    void OnNotifyCancelMatch(MsgPackage msg);
    
private:
    bool InitRedis();
    
    bool InitRecvMsgQueueThread();
    
    bool InitMatchQueue();
    
    static void* RecvProc(void* pParam);
    //初始化消息队列缓存
    bool InitMQRedis();
    //初始化连接服务消息队列
    bool InitConnMQRedis();
    
    void Match(const RoomType& roomType);
    
    string GenUserRawDataKey(int64 userID);
    UserRawData GetBaseInfo(int64 userID);
public:
	CRedisIntf*                         m_pRedis;
    bool                                m_bSvrdClose        ;  //服务器收到关闭的指令 ,即将关闭
    unordered_map<int32, MatchQueue> m_MathQueuesMap;
    MatchQueue m_mqPrimary;
    MatchQueue m_mqIntermediate;
    MatchQueue m_mqPremium;
    vector<RoomType> m_vRoomTypes = {RoomType::RT_PRIMARY, RoomType::RT_INTERMEDIATE, RoomType::RT_PREMIUM};
    HANDLE m_hMatchTimer;
};

#endif // _USER_HANDINFO_SERVICE_H_

