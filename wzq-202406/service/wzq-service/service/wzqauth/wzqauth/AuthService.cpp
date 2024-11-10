//
//  AuthService.cpp
//  authservice
//
//  Created by 刘挺 on 2022/11/24.
//

#include "AuthService.hpp"
#include "AppDef.h"
#include "log.h"
#include "ProtocolDef.h"
#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <openssl/sha.h>
#include <openssl/rand.h>
#include <ctime>
#include <mysql/mysql.h>
#include <map>
#include "util.hpp"
#include "statsd_client_singleton.hpp"

// 外显ID长度可配合redis发布订阅动态调整
#define DISPLAYID_LENGTH  6

CAuthService::CAuthService()
: m_hMySQL(NULL), m_bPrint(true)
{
	memset(m_szSQL, 0, MAX_SQL_LEN);
	m_hMySQL = mysql_init(0);
	if (NULL == m_hMySQL) {
        printf("初始化 MySQL 连接失败: %s\n", mysql_error(m_hMySQL));
        // 可以根据需要进行其他处理，比如抛出异常或者退出程序
    } else {
        printf("MySQL 连接初始化成功\n");
    }
}

CAuthService::~CAuthService()
{
    AuthClose();
}


int CAuthService::AuthConnect(const string &strHost, 
						   const string &strUser, 
						   const string &strPwd, 
						   const string &strDBName, 
						   unsigned int nPort, 
						   const string &strCharSet)
{

	if (m_hMySQL != mysql_real_connect(m_hMySQL, strHost.c_str(), strUser.c_str(), strPwd.c_str(), strDBName.c_str(), nPort, 0, 0))
	{
		printf("mysql_real_connect err(%s)\n", mysql_error(m_hMySQL));
		return SQL_FAILED;
	}

	if (mysql_set_character_set(m_hMySQL, strCharSet.c_str()))
	{
		printf("mysql_set_character_set err(%s)\n", mysql_error(m_hMySQL));
		return SQL_FAILED;
	}

	char value = 1 ;
	int ret = mysql_options(m_hMySQL, MYSQL_OPT_RECONNECT, (char *)&value);
	printf("mysql_options ret : %d", ret);

	return SQL_SUCCESS;
}

int CAuthService::AuthClose()
{
	if (m_hMySQL)
	{
		mysql_close(m_hMySQL);
		m_hMySQL = NULL;
	}

	mysql_library_end();

	return SQL_SUCCESS;
}

int CAuthService::AuthPing()
{
	if (mysql_ping(m_hMySQL))
	{
		printf("mysql_ping err(%s)\n", mysql_error(m_hMySQL));
		return SQL_FAILED;//
	}

	return SQL_SUCCESS;//
}

int CAuthService::AuthCommit()
{
    return mysql_commit(m_hMySQL);
}

int CAuthService::AuthRollback()
{
    return mysql_rollback(m_hMySQL);
}

string sha256_salt(const std::string& passwd, const std::string& salt){
    std::string salted_password = passwd + salt;
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, salted_password.c_str(), salted_password.length());
    SHA256_Final(hash, &sha256);

    std::stringstream ss;
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        ss << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
    }

    return ss.str();
}

void CAuthService::OnREQRegister(MsgPackage& msg) {
    //处理注册消息
    REQUserRegister req ;
    req.ParsePartialFromString(msg.msg());
    LOG("CAuthService", "OnREQRegister", "请求注册: 账号: %s, 密码: %s ", req.username().c_str() , req.userpassword().c_str());
    
    ACKRegister ack;
    int64 userID = 0 ;
    std::string password, saltHex, displayID;
    int nresult = m_pSampleDBPool->QueryUserInfo(req.username(), userID, password, saltHex, displayID);
    if(IsNotZero(nresult))
    {
        ack.set_code(register_result_failed_unknown);
    }
    if(userID > 0)
    {
        ack.set_code(register_result_failed_exists);
    }

    // 生成外显ID, 目前为6,支持3万用户左右。
    displayID = GenUserDisplayId(DISPLAYID_LENGTH);
    nresult = m_pSampleDBPool->InsertUserInfo(req.username(), req.userpassword(), displayID, "");
    if(IsNotZero(nresult))
    {
        LOG("CAuthService", "OnREQRegister", "注册失败, 插入数据出错");
        ack.set_code(register_result_failed_unknown);
    }
    ack.set_code(RESULT_CODE_SUCCESS);
    
    SendSvrdMsg(GetConnSvrdMsgRoute(), (MSGID_REGISTER | ID_ACK), 0, 0, ack.SerializeAsString(), msg.clientid());
}

void CAuthService::OnREQPWDLogin(MsgPackage& msg) {
    REQUserPWDLogin req ;
    req.ParsePartialFromString(msg.msg());
    LOG("CAuthService", "OnREQPWDLogin", "请求密码登陆: 账号: %s, 密码: %s ", req.username().c_str() , req.userpassword().c_str());
    ACKUserPWDLogin ack;
    int64 userID = 0;
    std::string password, saltHex, displayID;
    int nresult = m_pSampleDBPool->QueryUserInfo(req.username(), userID, password, saltHex, displayID);
    if(IsNotZero(nresult))
    {
        ack.set_result_id(login_result_failed_unknown);
    }
    if(sha256_salt(req.userpassword(),saltHex) != password)
    {
        ack.set_result_id(login_result_failed_passwd_error);
    }
    time_t now = time(0);
    tm *ltm = localtime(&now);
    std::string login_time = std::to_string(1900 + ltm->tm_year) + "-" + std::to_string(1 + ltm->tm_mon) + "-" + std::to_string(ltm->tm_mday) + " " + std::to_string(ltm->tm_hour) + ":" + std::to_string(ltm->tm_min) + ":" + std::to_string(ltm->tm_sec);
    
    memset(m_szSQL, 0, MAX_SQL_LEN);
    int len=sprintf(m_szSQL, "update user set login_time = '%s' where account = '%s'", login_time.c_str(), req.username().c_str());
    mysql_real_connect(m_hMySQL, "8.130.49.219", "root", "123456", "wzq", 3307, 0, 0);
    int ret = mysql_real_query(m_hMySQL, m_szSQL, len);
    if (IsNotZero(ret))
    {
        mcgWriteLog("Error exec query: %s \n", mysql_error(m_hMySQL));
    }

    std::string token = SecurityTool::generateToken(std::to_string(userID), "pwd", password);
    ack.set_result_id(RESULT_CODE_SUCCESS);
    ack.set_token(token.c_str());
  
    // 设置缓存信息到redis
    UserRawData baseInfo;

    baseInfo.set_userid(userID);
    m_pSampleDBPool->QueryUserRawDataInfo(userID, baseInfo);
    LOG("CAuthService", "OnREQPWDLogin", "uid:%d基本信息, name:%s, displayID:%s, socre:%d, highestScore:%d", 
    userID, baseInfo.name().c_str(), baseInfo.displayid().c_str(), baseInfo.score(), baseInfo.highestscore());
    
    if (baseInfo.has_gamerecord()) {
        LOG("CAuthService", "OnREQPWDLogin", "uid:%d对局数据, winCount:%d, totalCount:%d",
        userID, baseInfo.gamerecord().totalcount(),  baseInfo.gamerecord().wincount());
    }

    SetBaseInfo(baseInfo);
  
    // 传回基本信息
    auto ackBaseInfo = ack.mutable_baseinfo();
    *ackBaseInfo = baseInfo;

    LOG("CAuthService", "OnREQPWDLogin", "uid:%d, 密码校验通过, token为: %s", userID, token.c_str());
    SendSvrdMsg(GetConnSvrdMsgRoute(), (MSGID_PWD_LOGIN | ID_ACK), 0, userID, ack.SerializeAsString(), msg.clientid());
}

void CAuthService::OnREQTokenLogin(MsgPackage& msg) {
    REQUserTokenLogin req ;
    ACKUserTokenLogin ack;
    req.ParseFromString(msg.msg());

    string userID, password, timeStr, loginType;
    bool checkPass = true;
    SecurityTool::unpackToken(req.token(), loginType, userID, password, timeStr);
    time_t tokenTime;
    std::istringstream(timeStr) >> tokenTime;

    int64 uidNum = stol(userID);
    LOG("CAuthService", "OnREQTokenLogin", "uid:%d, 请求token登陆, token: %s", uidNum , req.token().c_str());

    time_t now = time(0);
    const int SECONDS_IN_30_DAYS = 30 * 24 * 60 * 60;
    if (difftime(now, tokenTime) > SECONDS_IN_30_DAYS) {
        LOG("CAuthService", "OnREQTokenLogin", "uid:%d, token:%s已过期, loginType:%s", uidNum, req.token().c_str(), loginType.c_str());
        checkPass = false;
    }

    string dbPassword, dbPhoneNumber, displayID;
    m_pSampleDBPool->QueryUserInfoByID(uidNum, dbPassword, dbPhoneNumber, displayID);

    // 不同登陆渠道验证方式不一样
    if (loginType == "pwd") {
        if(password != dbPassword)
        {
            LOG("CAuthService", "OnREQTokenLogin", "token验证失败:密码错误, password:%s, dbPassword:%s", password.c_str(), dbPassword.c_str());
            checkPass = false;
        }
    } else if (loginType == "phoneNumber") {
        if(password != dbPhoneNumber)
        {
            LOG("CAuthService", "OnREQTokenLogin", "token验证失败:手机号有误, phoneNumber:%s, dbPhoneNumber:%s", password.c_str(), dbPhoneNumber.c_str());
            checkPass = false;
        }
    } else {
        LOG("CAuthService", "OnREQTokenLogin", "token验证失败:未知渠道登陆, token:%s, loginType:%s", req.token().c_str(), loginType.c_str());
        checkPass = false;
    }

    if (checkPass) {
        ack.set_resultcode(RESULT_CODE_SUCCESS);

        // 传回基本信息
        auto baseInfo = ack.mutable_baseinfo();
        *baseInfo = GetBaseInfo(uidNum);
    }
    
    SendSvrdMsg(GetConnSvrdMsgRoute(), (MSGID_TOKEN_LOGIN | ID_ACK), 0, uidNum, ack.SerializeAsString(), msg.clientid());
}


bool CAuthService::VerifyCode(const std::string& phoneNumber, const std::string& code) {
    const std::string SMSPrefix = "sms_";
    const int SMSExpire = 300;

    // 获取所有验证码
    std::map<std::string, std::string> codeMap;
    cache()->hash_getall(SMSPrefix + phoneNumber, codeMap);

    for (const auto& kv : codeMap) {
        if (kv.first == code) {
            auto timestamp = std::stoll(kv.second);
            auto now = std::chrono::system_clock::now().time_since_epoch() / std::chrono::seconds(1);

            // 判断验证码是否在5分钟内有效期
            if (now - timestamp <= SMSExpire) {
                return true;
            } else {
                LOG("CAuthService", "VerifyCode", "验证码:%s已过期, phone:%s", code.c_str(), phoneNumber.c_str());
                return false;
            }
        }
    }

    LOG("CAuthService", "VerifyCode", "验证码:%s不正确, phone:%s", code.c_str(), phoneNumber.c_str());
    return false;
}

void CAuthService::OnREQPhoneNumberLogin(MsgPackage& msg) {
    REQPhoneNumberLogin req ;
    ACKPhoneNumberLogin ack;
    req.ParseFromString(msg.msg());

    // 验证验证码是否正确
    if (VerifyCode(req.phonenumber(), req.verifycode())) {
        LOG("CConnService", "OnREQPhoneNumberLogin", "验证码:%s校验成功, phone:%s", req.verifycode().c_str(), req.phonenumber().c_str());
        ack.set_resultcode(RESULT_CODE_SUCCESS);
    }
    
    // 如果初次登陆就走注册逻辑
    int64 userID = 0;
    int nresult = m_pSampleDBPool->QueryUserInfoByPhoneNumber(userID, req.phonenumber());

    if(IsNotZero(nresult))
    {
        ack.set_resultcode(RESULT_CODE_ERROR);
    }

    if(userID > 0)
    {
        LOG("CConnService", "OnREQPhoneNumberLogin", "phone:%s已存在, 可直接登录", req.phonenumber().c_str());
    } else {
        // 生成外显ID, 目前为6,支持3万用户左右。
        string displayID = "";
        displayID = GenUserDisplayId(DISPLAYID_LENGTH);
        nresult = m_pSampleDBPool->InsertUserInfo("", "", displayID, req.phonenumber());
        if(IsNotZero(nresult))
        {
            LOG("CAuthService", "OnREQRegister", "手机号注册失败, 插入数据出错");
            ack.set_resultcode(RESULT_CODE_ERROR);
            SendSvrdMsg(GetConnSvrdMsgRoute(), (MESSAGE_TYPE_PHONE_NUMBER_LOGIN | MESSAGE_TYPE_ACK), 0, userID, ack.SerializeAsString(), msg.clientid());
            return;
        } else {
            // 再次查询userID, 不做过多判断了
            m_pSampleDBPool->QueryUserInfoByPhoneNumber(userID, req.phonenumber());
            ack.set_resultcode(RESULT_CODE_SUCCESS);
        }
    }

    // 这里待优化， 用结构体包含这些参数
    string dbPassword, dbPhoneNumber, displayID;
    m_pSampleDBPool->QueryUserInfoByID(userID, dbPassword, dbPhoneNumber, displayID);

    std::string token = SecurityTool::generateToken(std::to_string(userID), "phoneNumber", req.phonenumber());
    ack.set_token(token.c_str());
  
    // 设置缓存信息到redis
    UserRawData baseInfo;
    baseInfo.set_userid(userID);
    baseInfo.set_displayid(displayID);
    baseInfo.set_name("新手玩家");
    SetBaseInfo(baseInfo);

    // 传回基本信息
    auto ackBaseInfo = ack.mutable_baseinfo();
    *ackBaseInfo = baseInfo;

    SendSvrdMsg(GetConnSvrdMsgRoute(), (MESSAGE_TYPE_PHONE_NUMBER_LOGIN | MESSAGE_TYPE_ACK), 0, userID, ack.SerializeAsString(), msg.clientid());
}

void CAuthService::OnREQSendVerifyCode(MsgPackage& msg) {
    REQSendVerifyCode req ;
    ACKSendVerifyCode ack;
    req.ParseFromString(msg.msg());
    Json::Value root;
    root["phoneNumber"] = req.phonenumber();
    Json::StreamWriterBuilder writer;
    std::string data = Json::writeString(writer, root);

    LOG("CAuthService", "OnREQSendVerifyCode", "收到发送验证码请求, phone:%s, detile:%s",
    req.phonenumber().c_str(), data.c_str());     
    mqw()->list_rpush(GetGMToolRoute(), data);

    // 通知客户端发送成功
    ack.set_resultcode(RESULT_CODE_SUCCESS);
    SendSvrdMsg(GetConnSvrdMsgRoute(), (MESSAGE_TYPE_SEND_VERIFY_CODE | MESSAGE_TYPE_ACK), 0, 0, ack.SerializeAsString(), msg.clientid());
}

void CAuthService::SetBaseInfo(UserRawData& baseInfo) {
    string key = GenUserRawDataKey(baseInfo.userid());
    cache()->hash_set(key, "displayID", baseInfo.displayid());
    cache()->hash_set(key, "name", baseInfo.name());
    cache()->hash_set(key, "score", to_string(baseInfo.score()));
    cache()->hash_set(key, "highestScore", to_string(baseInfo.highestscore()));
    LOG("CAuthService", "SetBaseInfo", "更新缓存, displayID:%s, name:%s, score:%d, highestScore:%d",
        baseInfo.displayid().c_str(), baseInfo.name().c_str(), baseInfo.score(), baseInfo.highestscore());
    
    // 现在简单做，单个数据作为value，比较主流的是一类数据作为value
    if (baseInfo.has_gamerecord()) { 
        const GameRecord& gameRecord = baseInfo.gamerecord();
        cache()->hash_set(key, "totalCount", to_string(gameRecord.totalcount()));
        cache()->hash_set(key, "winCount", to_string(gameRecord.wincount()));
        LOG("CAuthService", "SetBaseInfo", "uid:%d, 更新缓存, totalCount:%d, winCount:%d", 
        baseInfo.userid(), gameRecord.totalcount(), gameRecord.wincount());
    }
}

string CAuthService::GenUserRawDataKey(int64 userID) {
    char key[50];
    sprintf(key, "USER_RAW_DATA_%d", userID);
    return string(key);
}

 UserRawData CAuthService::GetBaseInfo(int64 userID) {
    string displayID, name, score, highestScore, totalCount, winCount;
    string key = GenUserRawDataKey(userID);
    cache()->hash_get(key, "displayID", displayID);
    cache()->hash_get(key, "name", name);
    cache()->hash_get(key, "score", score);
    cache()->hash_get(key, "highestScore", highestScore);

    // 传回基本信息
    UserRawData baseInfo;
    baseInfo.set_userid(userID);
    baseInfo.set_displayid(displayID);
    baseInfo.set_name(name);
    if (score != "") {
        baseInfo.set_score(stoi(score));
    } 
     if (highestScore != "") {
        baseInfo.set_highestscore(stoi(highestScore));
    } 
    
    cache()->hash_get(key, "totalCount", totalCount);
    cache()->hash_get(key, "winCount", winCount);
  
    if (totalCount != "") {
        auto gameRecord = baseInfo.mutable_gamerecord();
        gameRecord->set_totalcount(stoi(totalCount));
        gameRecord->set_wincount(stoi(winCount));
    } 
    
    return baseInfo;
 }    

//游戏结算
void CAuthService::OnNotifyGameSettlement(MsgPackage& msg){
    wzq::PlayingResult result;
    result.ParseFromString(msg.msg());
    int64 gameResult = result.gameresult();
    int64 winUserID = result.winuserid();
    int64 loseUserID = result.loseuserid();
    int64 Color;
    if(result.has_color())
    {
        Color=result.color();
    }
    else
    {
        Color=0;
    }
    std::vector<std::pair<int, int>> movesData;

    // 遍历 playingResult 中的 moves
    for (const auto& move : result.moves()) {
        int x = move.x();
        int y = move.y();
        movesData.push_back(std::make_pair(x, y));
    }
    if(movesData.size()==0)
    {
        movesData.push_back(std::make_pair(-1, -1));
    }
    LOG("CAuthService", "OnNotifyGameSettlement", "游戏结果:%d,获胜id:%d,失败id:%d",gameResult,winUserID,loseUserID);
    LOG("CAuthService", "OnNotifyGameSettlement","gameResult:%d,Color:%d,movesData:%d",gameResult,Color,movesData[0].first);
    int re= m_pSampleDBPool->UpdatePlayingResult(gameResult,winUserID,loseUserID,Color,movesData);
    
}

 // 获取排行榜数据
void CAuthService::OnREQLeaderBoardInfo(MsgPackage& msg) {
    
    REQLeaderboard req;
    ACKLeaderboard* ack = new ACKLeaderboard;
    req.ParseFromString(msg.msg());
    int64 userID = msg.user_id();
    *ack = CLeaderboard::GetBoard(req, "RankingBoard");
    LOG("CAuthService", "OnREQLeaderBoardInfo", "uid:%d请求获取排行榜数据, result:%d",msg.user_id(), (*ack).resultcode());
    SendSvrdMsg(GetConnSvrdMsgRoute(), (MESSAGE_TYPE_REQ_LEADER_BOARD_INFO | MESSAGE_TYPE_ACK), 0, userID, (*ack).SerializeAsString(), msg.clientid());
}

 // 修改昵称
void CAuthService::OnREQChangeName(MsgPackage& msg) {
    REQChangeName req;
    ACKChangeName ack;
    req.ParseFromString(msg.msg());
    int64 userID = msg.user_id();
    int result = m_pSampleDBPool->UpdateUserName(userID, req.name());
    LOG("CAuthService", "OnREQSendVerifyCode", "uid:%d请求修改昵称为:%s, result:%d",msg.user_id(), req.name().c_str(), result);
    // 更新redis缓存
    string key = GenUserRawDataKey(userID);
    cache()->hash_set(key, "name", req.name());

    ack.set_resultcode(result);
    SendSvrdMsg(GetConnSvrdMsgRoute(), (MESSAGE_TYPE_CHANGE_NAME | MESSAGE_TYPE_ACK), 0, userID, ack.SerializeAsString(), msg.clientid());
}

bool CAuthService::OnReceiveQueueMsg(BYTE* pbyBuffer, int nBufferLen)
{
    auto& client = StatsdClientSingleton::getInstance();
    client.increment("AuthServer.req.all.count");

    MsgPackage msg ;
    if(! msg.ParseFromArray(pbyBuffer, nBufferLen))
    {
        LOG("CConnService", "OnReceiveQueueMsg", "parse failed!", msg.msg_id());
        return false ;
    }

    LOG("CAuthService", "OnReceiveQueueMsg", "收到一条消息:%d!", msg.msg_id());

    switch (msg.msg_id())
    {
    case (MSGID_REGISTER | ID_REQ):
        OnREQRegister(msg);
        break;
    case (MSGID_PWD_LOGIN | ID_REQ):
        OnREQPWDLogin(msg);
        break;
    case (MSGID_TOKEN_LOGIN | ID_REQ):
        OnREQTokenLogin(msg);
        break;
    case (MESSAGE_TYPE_SEND_VERIFY_CODE | MESSAGE_TYPE_REQ):
        OnREQSendVerifyCode(msg);
        break;
    case (MESSAGE_TYPE_PHONE_NUMBER_LOGIN | MESSAGE_TYPE_REQ):
        OnREQPhoneNumberLogin(msg);
        break;
    case (MESSAGE_TYPE_CHANGE_NAME | MESSAGE_TYPE_REQ):
        OnREQChangeName(msg);
        break;
    case MESSAGE_TYPE_NOTIFY_GAME_SETTLEMENT:
        OnNotifyGameSettlement(msg);
        break;
    case MESSAGE_TYPE_REQ_LEADER_BOARD_INFO | MESSAGE_TYPE_REQ:
        OnREQLeaderBoardInfo(msg);
        break;
    default:
        LOG("CAuthService", "OnReceiveQueueMsg", "未知消息类型!", msg.msg_id());
        break;
    }

    // 服务器请求打点
    std::string statKey = StatsUtils::GetStatKeyFromMsgID(msg.msg_id());
    client.increment(statKey);

    return true;
}

//获取该服务器的路由配置
std::string CAuthService::GetAuthSvrdMsgRoute()
{
    return "WZQ_MSG_QUEUE_AUTHSVRD_" + getServiceID();
}

//获取该服务器的路由配置
std::string CAuthService::GetGMToolRoute()
{
    return "WZQ_GM_TOOL";
}

void* CAuthService::RecvProc(void* pParam)
{
    CAuthService* pService = (CAuthService*)(pParam);
    if(! pService) return nullptr ;
    std::string key = pService ->GetAuthSvrdMsgRoute();
    printf("CAuthService::RecvProc key : %s \n",key.c_str());
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

//获取连接服的路由信息
std::string CAuthService::GetConnSvrdMsgRoute()
{
    return "WZQ_MSG_QUEUE_CONNSVRD_conn101_01";
}

bool CAuthService::InitRecvMsgQueueThread()
{
    pthread_t nThreadID ;
    int ret = pthread_create(&nThreadID,NULL,CAuthService::RecvProc,this);
    return ret == 0 ;
}

bool CAuthService::OnInit()
{
    if (!CService::OnInit()) return false;
    //初始化读取消息队列的线程
    InitRecvMsgQueueThread();
    //初始化数据库
    InitDBService();
    //初始化Redis
    InitRedisService();
    return true;
}

redisContext* CAuthService::GetRedisIns() {
    return m_pRedisIns;
}

bool CAuthService::InitRedisService() {
    const char* hostname = "8.130.49.219";
    int port = 6379;
    struct timeval timeout = { 1, 500000 }; // 1.5 seconds
    m_pRedisIns = redisConnectWithTimeout(hostname, port, timeout);

    if (m_pRedisIns == nullptr || m_pRedisIns->err) {
        if (m_pRedisIns) {
            std::cerr << "Connection error: " << m_pRedisIns->errstr << std::endl;
            redisFree(m_pRedisIns);
        } else {
            std::cerr << "Connection error: can't allocate redis context" << std::endl;
        }
        return false;
    }

    // Authenticate with password
    redisReply* reply = (redisReply*)redisCommand(m_pRedisIns, "AUTH %s", "123456");
    if (reply == nullptr) {
        std::cerr << "Error during AUTH command: " << m_pRedisIns->errstr << std::endl;
        redisFree(m_pRedisIns);
        return false;
    }
    
    if (reply->type == REDIS_REPLY_ERROR) {
        std::cerr << "AUTH command failed: " << reply->str << std::endl;
        freeReplyObject(reply);
        redisFree(m_pRedisIns);
        return false;
    }

    freeReplyObject(reply);

    std::cout << "Redis connection initialized successfully!" << std::endl;
    return true;
}

bool CAuthService::InitDBService()
{
    char *szHost = g_oLuaIni.GetString(DB_HOST);
    int nDBPort = g_oLuaIni.GetInt(DB_PORT);
    char *szDBUserName = g_oLuaIni.GetString(DB_USERNAME);
    char *szDBPassword = g_oLuaIni.GetString(DB_PASSWORD);
    char *szDBName = g_oLuaIni.GetString(DB_NAME);
    int nDBConnNum = g_oLuaIni.GetInt(DB_CONN_NUM);
    if ((szHost == NULL) || (szDBUserName == NULL) || (szDBPassword == NULL) || (szDBName == NULL))
    {
        return false;
    }

    m_pSampleDBPool = new CSampleDBPool();
    if (!m_pSampleDBPool)
    {
        mcgWriteLog("new db sample pool failed");
        return false;
    }

     if (0 != m_pSampleDBPool->Connect(szHost, szDBUserName, szDBPassword, szDBName, nDBPort))
    {
        mcgWriteLog("init db sample pool failed");
        delete m_pSampleDBPool;
        m_pSampleDBPool = NULL;
        return false;
    }

    mcgWriteLog("InitDBService (IP=%s, Port=%d, UserName=%s, Pwd=%s, DBName=%s, ConnNum=%d) success",
                szHost, nDBPort, szDBUserName, szDBPassword, szDBName, nDBConnNum);
    return true;
}

int CAuthService::SendSvrdMsg(const std::string& msgroute , int64 msg_id , int64 seq_id , int64 user_id , const std::string& msg,int64 nClientID)
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

std::string CAuthService::GenUserDisplayId(int length) {
    std::random_device rd;  
    std::mt19937 gen(rd()); 
    std::uniform_int_distribution<> dis(0, 9);   
  
    std::string id;  
    for (int i = 0; i < length; ++i) {  
        id += std::to_string(dis(gen));   
    }  
  
    return id;  
}

// 生成统计键的函数
std::string CAuthService::GetStatKeyFromMsgID(int msgID) {
    std::string baseKey = "AuthServer.req.";

    // 消息类型到统计键的映射规则
    switch (msgID) {
        case MSGID_REGISTER:
            return baseKey + "REGISTER_REQ";
        case MSGID_PWD_LOGIN:
            return baseKey + "PWD_LOGIN_REQ";
        case MSGID_TOKEN_LOGIN:
            return baseKey + "TOKEN_LOGIN_REQ";
        case MESSAGE_TYPE_SEND_VERIFY_CODE | MESSAGE_TYPE_REQ:
            return baseKey + "VERIFY_CODE_REQ";
        case (MESSAGE_TYPE_PHONE_NUMBER_LOGIN | MESSAGE_TYPE_REQ):
            return baseKey + "PHONE_NUMBER_LOGIN_REQ";
        default:
            return baseKey + "OTHER";
    }
}
