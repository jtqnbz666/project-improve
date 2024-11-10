#include "RoomService.h"
#include "log.h"
#include "mcgevent.h"
#include "mcgcommon/datatype.h"
#include <boost/regex.hpp>
#include <json/json.h>
#include <random>
#include "statsd_client_singleton.hpp"

#define DB_HOST		             "DBHost"                  //
#define DB_PORT				     "DBPort"                  //
#define DB_USERNAME			     "DBUserName"       	   //
#define DB_PASSWORD			     "DBPassword"              //
#define DB_NAME				     "DBName"				   //
#define DB_CONN_NUM			     "DBConnNum"  			   //
#define REDIS_IP                 "RedisIP"
#define REDIS_PORT			     "RedisPort"
#define REDIS_PWD                "RedisPwd"

#define SHAREREDIS_IP			 "ShareRedisIP"            // 缓存服务IP
#define SHAREREDIS_PORT			 "ShareRedisPort"          // 缓存服务端口号
#define SHAREREDIS_PASSWD		 "ShareRedisPasswd"        // 缓存服务器密码

extern DWORD seqid_new();

CRoomService::CRoomService()
{
    m_bSvrdClose = false;
}

CRoomService::~CRoomService()
{
    if (IsNotNULL(m_hMatchTimer)) {
        CMcgTimer::DeleteTimer(m_hMatchTimer, true);
    }
}

IEventPump *CRoomService::mcgclient(DWORD dwIP,WORD wPort)
{
    return nullptr;
}


bool CRoomService::OnInit()
{
	if (!CService::OnInit()) return false;
    
	if (!InitRedis())
	{
		mcgWriteLog("初始化 redis 失败了");
		return false;
	}
    //初始化消息队列缓存
    if(! InitMQRedis())
    {
        mcgWriteLog("CRoomService::OnInit func>>InitMQRedis failed");
        return false;
    }
    //初始化读取消息队列的线程
    InitRecvMsgQueueThread();
    
    InitMatchQueue();
    
    m_hMatchTimer = CMcgTimer::CreateTimer((LPTHREAD_START_ROUTINE)PMatch, this, 1000, MATCH_FREQUENCE);
	return true;
}

bool CRoomService::InitRecvMsgQueueThread()
{
    pthread_t nThreadID ;
    int ret = pthread_create(&nThreadID,NULL,CRoomService::RecvProc,this);
    return ret == 0 ;
}

DWORD CRoomService::OnMatch()
{
    LOG("CRoomService", "OnMatch", "enter");
    for (auto& type : m_vRoomTypes) {
        Match(type);
    }
    return 0;
}

bool CRoomService::InitMatchQueue()
{
    m_MathQueuesMap[RoomType::RT_PRIMARY] = m_mqPrimary;
    m_MathQueuesMap[RoomType::RT_INTERMEDIATE] = m_mqIntermediate;
    m_MathQueuesMap[RoomType::RT_PREMIUM] = m_mqPremium;
    return true;
}

void* CRoomService::RecvProc(void* pParam)
{
    CRoomService* pService = (CRoomService*)(pParam);
    if(! pService) return nullptr ;
    std::string key = pService->GetRoomSvrdMsgRoute();
    while (! pService->SvrdNeedClose())
    {
        //消息队列
        std::string strBuffer;
        int nResult = pService->mqr()->list_blpop(key, strBuffer , 1);
        if(nResult != 0)
        {
            continue ;
        }
        mcgWriteLog("CRoomService::RecvProc recv one msg");
        pService->AddEventQueueMsg(strBuffer);
    }
    return nullptr ;
}

bool CRoomService::InitRedis()
{
    m_pRedis = cache();
//	char *szIP = g_oLuaIni.GetString(REDIS_IP);
//    mcgWriteLog("CRoomService::InitRedis ip : %s" , szIP);
//	if (NULL == szIP)
//	{
//		return false;
//	}
//    std::string strRedisIp = szIP;
//
//	int dwRedisPort = g_oLuaIni.GetInt(REDIS_PORT);
//    mcgWriteLog("CRoomService::InitRedis  dwRedisPort: %d" , dwRedisPort);
//    char* szPwd = g_oLuaIni.GetString(REDIS_PWD);
//    mcgWriteLog("CRoomService::InitRedis pwd : %s" , szPwd);
//    if (NULL == szPwd)
//    {
//		return false;
//    }
//    std::string strRedisPwd = szPwd;
//
//	m_pRedis = new CRedisIntf();
//	if (NULL == m_pRedis)
//	{
//		mcgWriteLog("new CRedisIntf failed");
//		return false;
//	}
//    if(! m_pRedis ->init(strRedisIp.c_str(), dwRedisPort, strRedisPwd.c_str() , 0))
//    {
//        mcgWriteLog("Error CGameService::InitRedis, init redis failed");
//        return false ;
//    }
	return true;   
}

bool CRoomService::OnTimeOut(DWORD dwEventID)
{
	if(CService::OnTimeOut(dwEventID)) return true;
	switch(dwEventID)
	{
	default:
		break;
	}
	return true;
}

void CRoomService::OnExitProcess(UINT uExitCode)
{
	CService::OnExitProcess(uExitCode);
}

bool CRoomService::OnReceiveQueueMsg(BYTE* pbyBuffer, int nBufferLen)
{
//    auto& client = StatsdClientSingleton::getInstance();
//    client.increment("ConnServer.req.all.count");
    
    MsgPackage msg ;
    if(! msg.ParseFromArray(pbyBuffer, nBufferLen))
    {
        LOG("CRoomService", "OnReceiveQueueMsg", "parse failed");
        return false ;
    }
    
    LOG("CRoomService", "OnReceiveQueueMsg", "收到一条消息:%d!", msg.msg_id());
    
    //处理消息
    switch (msg.msg_id()) 
    {
        case (MESSAGE_TYPE_START_MATCH | MESSAGE_TYPE_REQ) :
            OnREQMatchGame(msg);
            break;
        case MESSAGE_TYPE_CANCEL_MATCH:
            OnNotifyCancelMatch(msg);
            break;
        default:
            LOG("CRoomService", "OnReceiveQueueMsg", "未知消息类型!", msg.msg_id());
            break;
    }
    
    return true ;
}

void CRoomService::OnSvrdClose()
{
    m_bSvrdClose = true ;
}

bool CRoomService::SvrdNeedClose()
{
    return m_bSvrdClose ;
}

//初始化消息队列缓存
bool CRoomService::InitMQRedis()
{
    //todo
    return true ;
}

//获取该连接服务器的路由消息数据
std::string CRoomService::GetRoomSvrdMsgRoute()
{
    return "WZQ_MSG_QUEUE_ROOMSVRD_wzqroom_02";
}

//获取该连接服务器的路由消息数据
std::string CRoomService::GetConnSvrdMsgRoute()
{
    return "WZQ_MSG_QUEUE_CONNSVRD_conn101_01";
}

//获取GAME服务路由信息
std::string CRoomService::GetGameSvrdMsgRoute()
{
    return "WZQ_MSG_QUEUE_GAMESVRD_wzqgame_01";
}

void CRoomService::OnREQMatchGame(MsgPackage msg)
{
    REQMatch req;
    req.ParseFromString(msg.msg());
    LOG("CGameService", "OnREQMatchGame", "uid:%d, 请求匹配，匹配等级场为%d",msg.user_id(), req.roomtype());
    
    char key[50];
    sprintf(key, "USER_RAW_DATA_%d", msg.user_id());
    cache()->hash_set(key, "clientId", to_string(msg.clientid()));
    
    m_MathQueuesMap[req.roomtype()].push(msg.user_id());
}

void CRoomService::OnNotifyCancelMatch(MsgPackage msg)
{
    NotifyCancelMatch notify;
    notify.ParseFromString(msg.msg());
    LOG("CRoomService", "OnNotifyCancelMatch", "uid:%d, 通知取消匹配，匹配等级场为%d",msg.user_id(), notify.roomtype());
    
    if (notify.has_roomtype()) {
        m_MathQueuesMap[notify.roomtype()].remove(msg.user_id());
        return;
    }
    
    for (auto& type : m_vRoomTypes) {
        m_MathQueuesMap[type].remove(msg.user_id());
    }
}

int CRoomService::SendSvrdMsg(const std::string& msgroute , int64 msg_id , int64 seq_id , int64 user_id , const std::string& msg,int64 nClientID)
{
    if(msgroute != "")
    {
        MsgPackage package ;
        package.set_msg_id(msg_id);
        package.set_seq_id(seq_id);
        package.set_user_id(user_id);
        package.set_msg(msg);
        package.set_src(GetConnSvrdMsgRoute());
        package.set_clientid(nClientID);
        mqw()->list_rpush(msgroute, package.SerializeAsString());
    }
    return 0 ;
}

string CRoomService::GenUserRawDataKey(int64 userID) {
    char key[50];
    sprintf(key, "USER_RAW_DATA_%d", userID);
    return string(key);
}

UserRawData CRoomService::GetBaseInfo(int64 userID) {
    string displayID, name;
    string key = GenUserRawDataKey(userID);
    cache()->hash_get(key, "displayID", displayID);
    cache()->hash_get(key, "name", name);
    // 传回基本信息
    UserRawData baseInfo;
    baseInfo.set_userid(userID);
    baseInfo.set_displayid(displayID);
    baseInfo.set_name(name);
    return baseInfo;
 }

void CRoomService::Match(const RoomType& roomType)
{
    //LOG("CGameService", "OnREQMatchGame", "场次%d开始匹配!", roomType);
    
    MatchQueue* curMatchQueue = &m_MathQueuesMap[roomType];
    
    // todo 调试用
    //curMatchQueue->print();
    
    while (curMatchQueue->size() > 1) {
        int64 firstPlayerUid = curMatchQueue->front();
        curMatchQueue->pop();
        int64 secondPlayerUid = curMatchQueue->front();
        curMatchQueue->pop();
        
        LOG("CGameService", "OnREQMatchGame", "等级%d匹配成功 p1 = %d, p2 = %d!", roomType, firstPlayerUid, secondPlayerUid);
        
        // 通知GameService创建房间并开始游戏
        NotifyStartGame notifyStartGame;
        notifyStartGame.set_userid1(firstPlayerUid);
        notifyStartGame.set_userid2(secondPlayerUid);
        SendSvrdMsg(GetGameSvrdMsgRoute(), static_cast<int64>(MESSAGE_TYPE_NOTIFY_START_GAME), 0,
        0, notifyStartGame.SerializeAsString(), 0);
        
        // 通知客户端匹配成功
        ACKMatch ackFirstPlayer;
        ackFirstPlayer.set_resultcode(match_result_success);
        auto ackFirstPlayerBaseInfo = ackFirstPlayer.mutable_enemyuserinfo();
        *ackFirstPlayerBaseInfo = GetBaseInfo(secondPlayerUid);
        
        char key[50];
        sprintf(key, "USER_RAW_DATA_%d", firstPlayerUid);
        string firstPlayerClientIdStr;
        cache()->hash_get(key, "clientId", firstPlayerClientIdStr);
        int64 firstPlayerClientId = std::stoll(firstPlayerClientIdStr);
        SendSvrdMsg(GetConnSvrdMsgRoute(), static_cast<int64>(MESSAGE_TYPE_START_MATCH | MESSAGE_TYPE_ACK), 0,
        firstPlayerUid, ackFirstPlayer.SerializeAsString(), firstPlayerClientId);
        
        ACKMatch ackSecondPlayer;
        ackSecondPlayer.set_resultcode(match_result_success);
        auto ackSecondPlayerBaseInfo = ackSecondPlayer.mutable_enemyuserinfo();
        *ackSecondPlayerBaseInfo = GetBaseInfo(firstPlayerUid);
        
        char key1[50];
        sprintf(key1, "USER_RAW_DATA_%d", secondPlayerUid);
        string secondPlayerClientIdStr;
        cache()->hash_get(key, "clientId", secondPlayerClientIdStr);
        int64 secondPlayerClientId = std::stoll(secondPlayerClientIdStr);
        SendSvrdMsg(GetConnSvrdMsgRoute(), static_cast<int64>(MESSAGE_TYPE_START_MATCH | MESSAGE_TYPE_ACK), 0,
        secondPlayerUid, ackSecondPlayer.SerializeAsString(), secondPlayerClientId);
    }
}
