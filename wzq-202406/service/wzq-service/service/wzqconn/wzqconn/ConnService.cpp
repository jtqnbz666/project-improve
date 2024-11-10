#include "ConnService.h"
#include "ConnClient.h"
#include "log.h"
#include "mcgevent.h"
#include "mcgcommon/datatype.h"
#include <json/json.h>
#include "util.hpp"
#include "statsd_client_singleton.hpp"


#define DB_HOST		             "DBHost"                  
#define DB_PORT				     "DBPort"                  
#define DB_USERNAME			     "DBUserName"       	   
#define DB_PASSWORD			     "DBPassword"              
#define DB_NAME				     "DBName"				   
#define DB_CONN_NUM			     "DBConnNum"  			   
#define REDIS_IP                 "RedisIP"
#define REDIS_PORT			     "RedisPort"
#define REDIS_PWD                "RedisPwd"

#define SHAREREDIS_IP			 "ShareRedisIP"            // 缓存服务IP
#define SHAREREDIS_PORT			 "ShareRedisPort"          // 缓存服务端口号
#define SHAREREDIS_PASSWD		 "ShareRedisPasswd"        // 缓存服务器密码

using namespace wzq;

extern DWORD seqid_new();

CConnService::CConnService()
{
}

CConnService::~CConnService()
{
}

IEventPump *CConnService::mcgclient(DWORD dwIP,WORD wPort)
{
	CConnClient *pClient=new CConnClient(dwIP,wPort);
	return pClient;
}


bool CConnService::OnInit()
{
	if (!CService::OnInit()) return false;
	m_pPlayerMgr = new CPlayerMgr();
    
	if (!InitRedis())
	{
		mcgWriteLog("初始化 redis 失败了");
		return false;
	}
    //初始化读取消息队列的线程
    InitRecvMsgQueueThread();
	return true;
}

bool CConnService::InitRecvMsgQueueThread()
{
    pthread_t nThreadID ;
    int ret = pthread_create(&nThreadID,NULL,CConnService::RecvProc,this);
    return ret == 0 ;
}

void* CConnService::RecvProc(void* pParam)
{
    CConnService* pService = (CConnService*)(pParam);
    if(! pService) return nullptr ;
    std::string key = pService ->GetConnSvrdMsgRoute();
    while (true)
    {
        if(pService ->SvrdNeedClose()) break ;
        //消息队列
        std::string strBuffer;
        int nResult = pService ->mqr() ->list_blpop(key, strBuffer, 1);
        if(IsNotZero(nResult))
        {
            continue ;
        }
        
        pService ->AddEventQueueMsg(strBuffer);
    }
    return nullptr ;
}

bool CConnService::InitRedis()
{
    m_pRedis = cache();

	return true;   
}

bool CConnService::OnTimeOut(DWORD dwEventID)
{
	if(CService::OnTimeOut(dwEventID)) return true;
	switch(dwEventID)
	{
	default:
		break;
	}
	return true;
}

void CConnService::OnExitProcess(UINT uExitCode)
{
	CService::OnExitProcess(uExitCode);
}

void CConnService::OnACKRegister(MsgPackage msg) {
    ACKUserRegister ack;
    ack.ParseFromString(msg.msg());
    LOG("CConnService", "OnACKRegister", "client:%d, 注册结果:%d!",msg.clientid(), ack.result_id());
    SendClientMsg(0, msg.msg_id(), msg.msg(), msg.seq_id(), msg.clientid());
}

void CConnService::OnACKPWDLogin(MsgPackage msg) {
    //登录返回
    ACKUserPWDLogin ack;
    ack.ParseFromString(msg.msg());
    LOG("CConnService", "OnACKPWDLogin", "uid:%d, 密码登陆结果:%d!",msg.user_id(), ack.result_id());
    if(ack.result_id() == RESULT_CODE_SUCCESS)
    {
        CPlayer* cplayer = m_pPlayerMgr->CreatePlayer(msg.user_id(), nullptr, nullptr);
        auto it = m_mapClientList.find((unsigned)msg.clientid());
        ((CConnClient*)(it->second))->SetPlayer(cplayer) ;
        ((CConnClient*)(it->second))->SetLoginStatus(true);
        cplayer->m_pClient = (CConnClient*)it->second;
    }

    SendClientMsg(msg.user_id(), msg.msg_id(), msg.msg(), msg.seq_id(), msg.clientid());
}

void CConnService::OnACKTokenLogin(MsgPackage msg) {
    // token登陆返回结果
    ACKUserTokenLogin ack;
    ack.ParseFromString(msg.msg());
    LOG("CConnService", "OnACKTokenLogin", "uid:%d, token登陆结果:%d!",msg.user_id(), ack.resultcode());
    if(ack.resultcode() == RESULT_CODE_SUCCESS)
    {
        CPlayer* cplayer = m_pPlayerMgr->CreatePlayer(msg.user_id(), nullptr, nullptr);
        auto it = m_mapClientList.find((unsigned)msg.clientid());
        ((CConnClient*)(it->second))->SetPlayer(cplayer) ;
        ((CConnClient*)(it->second))->SetLoginStatus(true);
        cplayer->m_pClient = (CConnClient*)it->second;
    }

    SendClientMsg(msg.user_id(), msg.msg_id(), msg.msg(), msg.seq_id(), msg.clientid());
}

void CConnService::OnACKPhoneNumberLogin(MsgPackage msg) {
    // token登陆返回结果
    ACKPhoneNumberLogin ack;
    ack.ParseFromString(msg.msg());
    LOG("CConnService", "OnACKPhoneNumberLogin", "uid:%d, phoneNumber登陆结果:%d!",msg.user_id(), ack.resultcode());
    if(ack.resultcode() == RESULT_CODE_SUCCESS)
    {
        CPlayer* cplayer = m_pPlayerMgr->CreatePlayer(msg.user_id(), nullptr, nullptr);
        auto it = m_mapClientList.find((unsigned)msg.clientid());
        if (it == m_mapClientList.end()) {
            LOG("CConnService", "OnACKPhoneNumberLogin", "fail to get ConnClient");
            return;
        }
        ((CConnClient*)(it->second))->SetPlayer(cplayer) ;
        ((CConnClient*)(it->second))->SetLoginStatus(true);
        cplayer->m_pClient = (CConnClient*)it->second;
    }

    SendClientMsg(msg.user_id(), msg.msg_id(), msg.msg(), msg.seq_id(), msg.clientid());
}

void CConnService::OnACKCreateTable(MsgPackage msg) {
    ACKCreateTable ack;
    ack.ParseFromString(msg.msg());
    LOG("CConnService", "OnACKCreateTable", "uid:%d, 创建桌子回应:%d!", msg.user_id(), ack.resultcode());
    SendClientMsg(msg.user_id(), msg.msg_id(), msg.msg(), msg.seq_id(), msg.clientid());
}

void CConnService::OnACKJoinTable(MsgPackage msg) {
    ACKJoinTable ack;
    ack.ParseFromString(msg.msg());
    LOG("CConnService", "OnACKJoinTable", "uid:%d, 加入桌子回应:%d!", msg.user_id(), ack.resultcode());
    SendClientMsg(msg.user_id(), msg.msg_id(), msg.msg(), msg.seq_id(), msg.clientid());
}

void CConnService::OnACKStartGame(MsgPackage msg) {
    ACKStartGame ack;
    ack.ParseFromString(msg.msg());
    LOG("CConnService", "OnACKStartGame", "uid:%d, 开始游戏回应:%d!", msg.user_id(), ack.resultcode());
    SendClientMsg(msg.user_id(), msg.msg_id(), msg.msg(), msg.seq_id(), msg.clientid());
}

void CConnService::OnACKReadyGame(MsgPackage msg) {
    ACKStartGame ack;
    ack.ParseFromString(msg.msg());
    LOG("CConnService", "OnACKReadyGame", "uid:%d, 请求准备回应:%d!", msg.user_id(), ack.resultcode());
    SendClientMsg(msg.user_id(), msg.msg_id(), msg.msg(), msg.seq_id(), msg.clientid());
}


 void CConnService::OnNotifyStartGame(MsgPackage msg) {
     NotifyStartGame notifyMsg;
     notifyMsg.ParseFromString(msg.msg());
     LOG("CConnService", "OnNotifyStartGame", "通知uid:%d, 游戏开始!", msg.user_id());
     SendClientMsg(msg.user_id(), msg.msg_id(), msg.msg(), msg.seq_id(), msg.clientid());
 }


void CConnService::OnUpdateTableStatus(MsgPackage msg) {
    LOG("CConnService", "OnUpdateTableStatus", "通知uid:%d, 更新游戏桌状态!", msg.user_id());
    SendClientMsg(msg.user_id(), msg.msg_id(), msg.msg(), msg.seq_id(), msg.clientid());
}

void CConnService::OnACKSendVerifyCode(MsgPackage msg) {
    LOG("CConnService", "OnACKSendVerifyCode", "uid:%d, 成功发送验证码!", msg.user_id());
    SendClientMsg(msg.user_id(), msg.msg_id(), msg.msg(), msg.seq_id(), msg.clientid());
}


void CConnService::OnACKMatch(MsgPackage msg)
{
    LOG("CConnService", "OnACKMatch", "uid:%d, 成功匹配!", msg.user_id());
    SendClientMsg(msg.user_id(), msg.msg_id(), msg.msg(), msg.seq_id(), msg.clientid());
}

void CConnService::OnACKChangeName(MsgPackage msg) {
    LOG("CConnService", "OnACKChangeName", "uid:%d, 修改昵称回应!", msg.user_id());
    SendClientMsg(msg.user_id(), msg.msg_id(), msg.msg(), msg.seq_id(), msg.clientid());
}


void CConnService::OnACKDrawChess(MsgPackage msg){
    LOG("CConnService", "OnACKDrawChess", "uid:%d, 成功发送棋子!", msg.user_id());
    SendClientMsg(msg.user_id(), msg.msg_id(), msg.msg(), msg.seq_id(), msg.clientid());
}

void CConnService::OnGameOver(MsgPackage msg){
     LOG("CConnService", "OnGameOver", "uid:%d, 发送胜利信息!", msg.user_id());
    SendClientMsg(msg.user_id(), msg.msg_id(), msg.msg(), msg.seq_id(), msg.clientid());
}

void CConnService::OnACKRegretChess(MsgPackage msg){
    LOG("CConnService", "OnACKRegretChess", "uid:%d, 发送悔棋请求!", msg.user_id());
    SendClientMsg(msg.user_id(), msg.msg_id(), msg.msg(), msg.seq_id(), msg.clientid());
}

void CConnService::OnACKAgreeRegretChess(MsgPackage msg){
    LOG("CConnService", "OnACKAgreeRegretChess", "uid:%d, 发送同意悔棋请求!", msg.user_id());
    SendClientMsg(msg.user_id(), msg.msg_id(), msg.msg(), msg.seq_id(), msg.clientid());
}

void CConnService::OnACKDisagreeRegretChess(MsgPackage msg){
    LOG("CConnService", "OnACKDisagreeRegretChess", "uid:%d, 发送不同意悔棋请求!", msg.user_id());
    SendClientMsg(msg.user_id(), msg.msg_id(), msg.msg(), msg.seq_id(), msg.clientid());
}

void CConnService::OnNotifyASKPeaceChess(MsgPackage msg){
    LOG("CConnService", "OnNotifyASKPeaceChess", "uid:%d, 发送和棋请求!", msg.user_id());
    SendClientMsg(msg.user_id(), msg.msg_id(), msg.msg(), msg.seq_id(), msg.clientid());
}

void CConnService::OnNotifyASKDisagreePeaceChess(MsgPackage msg){
    LOG("CConnService", "OnNotifyASKDisagreePeaceChess", "uid:%d, 发送不同意和棋通知!", msg.user_id());
    SendClientMsg(msg.user_id(), msg.msg_id(), msg.msg(), msg.seq_id(), msg.clientid());
}

void CConnService::OnACKLeaderBoard(MsgPackage msg){
    LOG("CConnService", "OnACKLeaderBoard", "uid:%d, 收到排行榜返回消息!", msg.user_id());
    SendClientMsg(msg.user_id(), msg.msg_id(), msg.msg(), msg.seq_id(), msg.clientid());
}

bool CConnService::OnReceiveQueueMsg(BYTE* pbyBuffer, int nBufferLen)
{
    auto& client = StatsdClientSingleton::getInstance();
    client.increment("ConnServer.req.all.count");

    MsgPackage msg ;
    if(! msg.ParseFromArray(pbyBuffer, nBufferLen))
    {
        LOG("CConnService", "OnReceiveQueueMsg", "parse failed!", msg.msg_id());
        return false ;
    }
    
    LOG("CConnService", "OnReceiveQueueMsg", "收到一条消息:%d!", msg.msg_id());

    switch (msg.msg_id())
    {
    case (MSGID_REGISTER | ID_ACK):
        OnACKRegister(msg);
        break;
    case (MSGID_PWD_LOGIN | ID_ACK):
        OnACKPWDLogin(msg);
        break;
    case (MSGID_TOKEN_LOGIN | ID_ACK):
        OnACKTokenLogin(msg);
        break;
    case (MESSAGE_TYPE_CREATE_TABLE | MESSAGE_TYPE_ACK):
        OnACKCreateTable(msg);
        break;
    case (MESSAGE_TYPE_JOIN_TABLE | MESSAGE_TYPE_ACK):
        OnACKJoinTable(msg);
        break;
    case (MESSAGE_TYPE_START_GAME | MESSAGE_TYPE_ACK):
        OnACKStartGame(msg);
        break;
    case (MESSAGE_TYPE_READY_GAME | MESSAGE_TYPE_ACK):
        OnACKReadyGame(msg);
        break;

     case MESSAGE_TYPE_NOTIFY_START_GAME:
        OnNotifyStartGame(msg);
         break;

    case MESSAGE_TYPE_UPDATE_TABLE_STATUS:
        OnUpdateTableStatus(msg);
        break;
    case (MESSAGE_TYPE_SEND_VERIFY_CODE | MESSAGE_TYPE_ACK):
        OnACKSendVerifyCode(msg);
        break;
    case (MESSAGE_TYPE_PHONE_NUMBER_LOGIN | MESSAGE_TYPE_ACK):
        OnACKPhoneNumberLogin(msg);
        break;

    case (MESSAGE_TYPE_START_MATCH | MESSAGE_TYPE_ACK):
        OnACKMatch(msg);
        break;
    case (MESSAGE_TYPE_CHANGE_NAME | MESSAGE_TYPE_ACK):
        OnACKChangeName(msg);

    case (MESSAGE_TYPE_MY_DRAW_CHESS ):
        OnACKDrawChess(msg);
        break;
    case (MESSAGE_TYPE_GAME_OVER):
        OnGameOver(msg);
        break;
    case (MESSAGE_TYPE_ASK_REGRET_CHESS | MESSAGE_TYPE_ACK):
        OnACKRegretChess(msg);
        break;
    case (MESSAGE_TYPE_NOTIFY_AGREE_REGRET_CHESS | MESSAGE_TYPE_ACK):
        OnACKAgreeRegretChess(msg);
        break;
    case (MESSAGE_TYPE_NOTIFY_DISAGREE_REGRET_CHESS | MESSAGE_TYPE_ACK):
        OnACKDisagreeRegretChess(msg);
        break;
    case MESSAGE_TYPE_NOTIFY_ASK_PEACE_CHESS:
        OnNotifyASKPeaceChess(msg);
        break;
    case MESSAGE_TYPE_NOTIFY_ASK_DISAGREE_PEACE_CHESS:
        OnNotifyASKDisagreePeaceChess(msg);
        break;
    case (MESSAGE_TYPE_REQ_LEADER_BOARD_INFO | MESSAGE_TYPE_ACK):
        OnACKLeaderBoard(msg);
        break;
    default:
        LOG("CAuthService", "OnReceiveQueueMsg", "未知消息类型!", msg.msg_id());
        break;
    }

    // 服务器请求打点
    std::string statKey = StatsUtils::GetStatKeyFromMsgID(msg.msg_id());
    client.increment(statKey);

    return true ;
}

bool CConnService::OnMsg(DWORD dwMsgID, DWORD dwSeqID)
{
	if (CService::OnMsg(dwMsgID, dwSeqID)) return true;
	
	switch(dwMsgID)
	{
        default:
			break;
	}

	mcgWriteLog("recieve message:0x%08X, seqid:%u in CUserHandInfoService::OnMsg()", dwMsgID, dwSeqID);
	
	
	return true;
}

void CConnService::OnSvrdClose()
{
    m_bSvrdClose = true ;
}

bool CConnService::SvrdNeedClose()
{
    return m_bSvrdClose ;
}

//初始化消息队列缓存
bool CConnService::InitMQRedis()
{
    REDIS_CONFIG _cache_conf = getCacheConf();
    
    mcgWriteLog("Info CGameService::InitMQRedis, IP:(%s), port:%u", _cache_conf.ip.c_str(), _cache_conf.port);
    return true ;
}

//获取该连接服务器的路由消息数据
std::string CConnService::GetConnSvrdMsgRoute()
{
    return "WZQ_MSG_QUEUE_CONNSVRD_" + getServiceID();
}

//获取GAME服务路由信息
std::string CConnService::GetGameSvrdMsgRoute()
{
    return "WZQ_MSG_QUEUE_GAMESVRD_wzqgame_01";
}

//获取鉴权服务路由信息
std::string CConnService::GetAuthSvrdMsgRoute()
{
    return "WZQ_MSG_QUEUE_AUTHSVRD_wzqauth_01";
}

//获取房间服务路由信息
std::string CConnService::GetRoomSvrdMsgRoute()
{
    return "WZQ_MSG_QUEUE_ROOMSVRD_wzqroom_02";
}

int CConnService::SendSvrdMsg(const std::string& msgroute , int64 msg_id , int64 seq_id , int64 user_id , const std::string& msg,int64 nClientID)
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
        mqw() ->list_rpush(msgroute, package.SerializeAsString());
    }
    return 0 ;
}

int CConnService::SendClientMsg(int64 userID , DWORD dwMsgID, const std::string& strBuffer, DWORD dwSeqID , int64 clientID)
{
    LOG("CConnService", "SendClientMsg", "返回客户端消息: userID:%d, clientID %d", userID, clientID);
    if(userID <= 0)
    {
        // 注册的时候会走这里
        auto it = m_mapClientList.find((unsigned)clientID);
        if(it != m_mapClientList.end())
        {
            ((CMCGClient*)it->second)->SendMsg(dwMsgID, strBuffer, dwSeqID);
        }
        return 0;
    }
    
    auto player =  m_pPlayerMgr->GetPlayer(userID);
    if(player)
    {
        player ->SendMsg(dwMsgID, strBuffer, dwSeqID);
    } else {
        LOG("CConnService", "SendClientMsg", "未找到玩家信息: userID:%d, clientID %d", userID, clientID);
    }
    return 0 ;
}
