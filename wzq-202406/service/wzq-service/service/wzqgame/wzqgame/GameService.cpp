#include "GameService.h"

#include "log.h"
#include "luaini.h"
#include "splitstring.h"
#include <sstream>
#include "globalfunc.h"
#include "mcgevent.h"
#include "TableMgr.h"
#include "PlayerMgr.h"
#include "Table.h"
#include "ProtocolDef.h"
#include <boost/regex.hpp>
#include <json/json.h>
#include "Player.h"
#include "WZQGame.h"
#include "tool/util.hpp"
#include "statsd_client_singleton.hpp"

extern CLuaIni g_oLuaIni;

#define CONTEXT_SEP				 " "							
#define APP_SERVICE_NAME		 "AppServiceName"				
#define APP_LOG_PATH			 "AppLogPath"					
#define GAMEINFO_SERVICE_IP		 "GameInfoSvrdIP"				
#define GAMEINFO_SERVICE_PORT	 "GameInfoSvrdPort"				
#define AI_SERVICE_COUNT		 "AiSvrdCount"					
#define AI_SERVICE_IP			 "AiSvrdIP"						
#define AI_SERVICE_PORT			 "AiSvrdPort"					
#define CLEAR_DATA				 "ClearData"					
#define REDIS_IP				 "RedisIP"						
#define REDIS_PORT				 "RedisPort"					
#define REDIS_PASSWD			 "RedisPasswd"					
#define CLEAR_SHAREDATA			 "ClearShareData"					// 是否清除缓存数据（0[]=不是,1[]=是）
#define SHAREREDIS_IP			 "ShareRedisIP"						// 缓存服务IP
#define SHAREREDIS_PORT			 "ShareRedisPort"					// 缓存服务端口号
#define SHAREREDIS_PASSWD		 "ShareRedisPasswd"					// 缓存服务器密码

#define TIMER_SCAN_REQSIT         (TIMER_GAMESERVER+2)
#define TIMER_KEEPALIVE           (TIMER_GAMESERVER+3)
CGameService::CGameService()
{
	m_bInitOk = false;

	m_pPlayerMgr = NULL;
	m_pTableMgr = NULL;
	m_pRedis = NULL;
	m_pLogicLog =  NULL;
    m_bSvrdClose = false ;
	 
}

CGameService::~CGameService()
{
	if (IsNotNULL(m_hTrySendTimer))
	{
		CMcgTimer::DeleteTimer(m_hTrySendTimer, true);
	}

	m_bInitOk = false;
}

bool CGameService::OnInit()
{
	if (!CService::OnInit()) return false;
	m_pTableMgr = new CTableMgr();
	m_pPlayerMgr = new CPlayerMgr();

	if (!InitAppLog())
	{
		mcgWriteLog("Error CGameService::OnInit, call InitAppLog failed");
		return false;
	}

    if(!InitSvrdID())
    {
        mcgWriteLog("Error CGameService::OnInit, call  GetSvrdID failed");
        return false ;
    }

	if (!InitRedis())
	{
		mcgWriteLog("Error CGameService::OnInit, call InitRedis failed");
		return false;
	}
    
    //初始化消息队列缓存
    if(! InitMQRedis())
    {
        mcgWriteLog("OnInit func>>InitMQRedis failed");
        return false;
    }
    
	m_hTrySendTimer = CMcgTimer::CreateTimer(NULL, this, 0, 10*1000, TIMER_GAMESERVER+1);
	if (IsNULL(m_hTrySendTimer))
	{
		mcgWriteLog("Error CGameService::OnInit, call CreateTimer failed");
		return false;
	}
    //初始化读取消息队列的线程
    InitRecvMsgQueueThread();
	m_bInitOk = true;
	printf("Init service success\n");
	return true;
}

bool CGameService::OnTimeOut(DWORD dwEventID)
{
	if(CService::OnTimeOut(dwEventID)) return true;
	return true;
}

void CGameService::UpLogLevel()
{
	if (IsNotNULL(m_pLogicLog))
	{
		m_pLogicLog->UpLevel();
	}
}

void CGameService::DownLogLevel()
{
	if (IsNotNULL(m_pLogicLog))
	{
		m_pLogicLog->DownLevel();
	}
}

void CGameService::OnSvrdClose()
{
    m_bSvrdClose = true ;
}

bool CGameService::SvrdNeedClose()
{
    return m_bSvrdClose ;
}

void CGameService::OnExitProcess(UINT uExitCode)
{
	printf("Error CGameService::OnExitProcess\n");
	CService::OnExitProcess(uExitCode);
}

CPlayer* CGameService::GetPlayerByID(int64 user_id)
{
	return m_pPlayerMgr->GetPlayer(user_id);
}

CTable* CGameService::GetTableByID(string tableID)
{
	return m_pTableMgr->GetTable(tableID);
}

int CGameService::GetTotalPlayer()
{
	return m_pPlayerMgr->Size();
}
int CGameService::GetTotalTable()
{
	return m_pTableMgr->Size();
}

bool CGameService::InitAppLog()
{
	//
	char *szRet = g_oLuaIni.GetString(APP_LOG_PATH);
    char *szAppServiceName = const_cast<char *>(getServiceName().c_str());
	if (IsNULL(szRet))
	{
		m_strAppLogPath = "./";
	}
	else
	{
		m_strAppLogPath = szRet;
	}
	m_strAppServiceName = szAppServiceName;

	char szServiceName[MAX_PATH];
	GetAppName(szServiceName);

	m_pLogicLog = new CLog();
	if (IsNULL(m_pLogicLog) || !m_pLogicLog->Init(szAppServiceName,"./",LOG_LEVEL_INFO))
	{
		mcgWriteLog("CGameService::InitAppLog, init log failed");
		return false;
	}

	return true;
}

bool CGameService::InitRedis()
{
    m_pRedis = cache();
	int iClear = g_oLuaIni.GetInt(CLEAR_DATA);
	if(iClear != 0)
	{
		// todo
	}
	mcgWriteLog("Info CGameService::InitRedis, IP:(%s), port:%d", getCacheConf().ip.c_str(), getCacheConf().port);
	return true;
}

//初始化消息队列缓存
bool CGameService::InitMQRedis()
{
    m_pMQRedis = mqw();

    REDIS_CONFIG _cache_conf = getCacheConf();
    mcgWriteLog("Info CGameService::InitMQRedis, IP:(%s), port:%u", _cache_conf.ip.c_str(), _cache_conf.port);
    return true ;
}

bool CGameService::InitSvrdID()
{
    m_strSvrdID = getServiceID();
    return true ;
}

void CGameService::OnREQCreateTable(MsgPackage msg) {
	REQCreateTable req;
	req.ParseFromString(msg.msg());

	string displayID = "", userName = "";
	char key[50];
	sprintf(key, "USER_RAW_DATA_%d", msg.user_id());
	cache()->hash_get(key, "displayID", displayID);
	LOG("CGameService", "OnREQCreateTable", "uid:%d, 请求创建桌子:%s!\n",msg.user_id(), displayID.c_str());
	
	// 先创建桌子
	auto table = m_pTableMgr->CreateTable(displayID);
	// 把创建者加入到桌子中
	CPlayer* player = m_pPlayerMgr->CreatePlayer(msg.user_id());
	player->m_pService = this;
	player->m_sTableID = displayID;
	table->m_pTableMaster = player;
	table->m_pGame->m_pPlayer1 = player;
	// 使用随机数生成器来随机选择颜色
	std::random_device rd;
	std::mt19937 gen(rd());
	// 0代表黑色，1代表白色

	//player->m_cColor = (dis(gen) == 0) ? ChessColorType::Chess_Black : ChessColorType::Chess_White;
	player->m_bIsReady = true;

	LOG("CGameService", "OnREQCreateTable", "uid:%d, 棋子颜色为:%d, isReady:%d!\n",msg.user_id(), player->m_cColor, player->m_bIsReady);

	std::uniform_int_distribution<> dis(1, 2); 
	player->m_cColor = (dis(gen) == 2) ? ChessColorType::Chess_Black : ChessColorType::Chess_White;
	player->m_bIsReady = true;
	player->m_eGameState=GS_UNREADY;

	LOG("CGameService", "OnREQCreateTable", "uid:%d, 棋子颜色为:%d, isReady:%d!\n",msg.user_id(), player->m_cColor, player->m_eGameState);

	// 创建好后通知ConnSever进一步通知客户端
	ACKCreateTable ack;
	ack.set_tableid(displayID);
	ack.set_resultcode(RESULT_CODE_SUCCESS);
	SendSvrdMsg(GetConnSvrdMsgRoute(), static_cast<int64>(MESSAGE_TYPE_CREATE_TABLE | MESSAGE_TYPE_ACK), 0, 
	static_cast<int64>(msg.user_id()), ack.SerializeAsString(), msg.clientid());
}

void CGameService::OnREQJoinTable(MsgPackage msg) {
	REQJoinTable req;
	ACKJoinTable ack;
	req.ParseFromString(msg.msg());

	// 找桌子
	auto table = m_pTableMgr->GetTable(req.tableid());
	LOG("CGameService", "OnREQJoinTable", "uid:%d, 请求加入桌子:%s!",msg.user_id(), req.tableid().c_str());
	
	if (table == nullptr) {
		LOG("CGameService", "OnREQJoinTable", "uid:%d, 没有找到桌子:%s!",msg.user_id(), req.tableid().c_str());
	} else {
		string key = GenUserRawDataKey(table->m_pTableMaster->m_nUserID);
		string name = "";
		cache()->hash_get(key, "name", name);
		CPlayer* player = m_pPlayerMgr->CreatePlayer(static_cast<int64>(msg.user_id()));
		player->m_pService = this;
		player->m_sTableID = req.tableid();
		player->m_cColor = table->m_pTableMaster->m_cColor == ChessColorType::Chess_Black ? ChessColorType::Chess_White : ChessColorType::Chess_Black;
		
		table->m_pGame->m_pPlayer2 = player;
		table->m_pGame->m_pPlayer1->m_eGameState=GS_READY;
		table->m_pGame->m_pPlayer2->m_eGameState=GS_UNREADY;
		ack.set_resultcode(RESULT_CODE_SUCCESS);

		LOG("CGameService", "OnREQJoinTable", "uid:%d, tableID:%d, isReady:%d!",msg.user_id(), req.tableid().c_str(), player->m_bIsReady);
		player->m_bIsReady = 0;

		LOG("CGameService", "OnREQJoinTable", "uid:%d, tableID:%d, isReady:%d!",msg.user_id(), req.tableid().c_str(), player->m_eGameState);
		player->m_bIsReady = 0;

	}

	
	SendSvrdMsg(GetConnSvrdMsgRoute(), static_cast<int64>(MESSAGE_TYPE_JOIN_TABLE | MESSAGE_TYPE_ACK), 0, static_cast<int64>(msg.user_id()), ack.SerializeAsString(), msg.clientid());
	// 同步最新状态
	table->m_pGame->OnUpdateTableStatus();
}

void CGameService::OnREQReadyGame(MsgPackage msg) {
	REQReadyGame req;
	ACKReadyGame ack;
	req.ParseFromString(msg.msg());
	LOG("CGameService", "OnREQReadyGame", "uid:%d, 请求准备!",msg.user_id());
	
	auto player = m_pPlayerMgr->GetPlayer(msg.user_id());
	auto table = m_pTableMgr->GetTable(player->m_sTableID);
	if (table == nullptr) {
		LOG("CGameService", "OnREQReadyGame", "uid:%d, 无法准备, 没有游戏桌%s!",msg.user_id(), player->m_sTableID.c_str());
	} else {
		player->m_bIsReady = true;
		player->m_eGameState=GS_READY;
		table->m_pGame->m_pPlayer1->m_eGameState=GS_READY;
	}

	ack.set_resultcode(RESULT_CODE_SUCCESS);
	SendSvrdMsg(GetConnSvrdMsgRoute(), static_cast<int64>(MESSAGE_TYPE_READY_GAME | MESSAGE_TYPE_ACK), 0, static_cast<int64>(msg.user_id()), ack.SerializeAsString(), msg.clientid());
	// 同步最新状态
	LOG("CGameService", "OnREQReadyGame", "uid:%d, 状态:%d",msg.user_id(), player->m_eGameState);
	table->m_pGame->OnUpdateTableStatus();
}

void CGameService::OnREQStartGame(MsgPackage msg) {
	REQStartGame req;
	req.ParseFromString(msg.msg());
	LOG("CGameService", "OnREQStartGame", "uid:%d, 请求开始游戏!",msg.user_id());
	
	auto player = m_pPlayerMgr->GetPlayer(msg.user_id());
	auto table = m_pTableMgr->GetTable(player->m_sTableID);
	ACKStartGame ack;
	if (table == nullptr) {
		LOG("CGameService", "OnREQReadyGame", "uid:%d, 无法开始游戏, 没有游戏桌%s!",msg.user_id(), player->m_sTableID.c_str());
	}
	else{
				ack.set_resultcode(RESULT_CODE_SUCCESS);
		// 通知桌主准备结果
		SendSvrdMsg(GetConnSvrdMsgRoute(), static_cast<int64>(MESSAGE_TYPE_START_GAME | MESSAGE_TYPE_ACK), 0, static_cast<int64>(msg.user_id()), ack.SerializeAsString(), msg.clientid());
		if (ack.resultcode() == RESULT_CODE_SUCCESS) {
			// 通知本桌玩家开始游戏

			//table->m_pGame->OnGameStart();

			player->m_eGameState=GS_PLAYING;
			table->m_pGame->m_pPlayer2->m_eGameState=GS_PLAYING;
			LOG("CGameService", "OnREQStartGame", "uid:%d, 状态:%d",msg.user_id(), player->m_eGameState);
			LOG("CGameService", "OnREQStartGame", "uid:%d, 客人状态:%d",msg.user_id(), table->m_pGame->m_pPlayer2->m_eGameState);
			table->m_pGame->OnUpdateTableStatus();
            table->m_pGame->OnGameStart();
		}
	}
	}

void CGameService::OnNotifyStartGame(MsgPackage msg)
{
    NotifyStartGame notify;
    notify.ParseFromString(msg.msg());
    LOG("CGameService", "OnNotifyStartGame", "uid1:%d, uid2:%d, 匹配成功!",notify.userid1(), notify.userid2());
    
    // 创建桌子
    string tableId = to_string(notify.userid1()) + to_string(notify.userid2());
    auto table = m_pTableMgr->CreateTable(tableId);
    
    // 创建匹配成功的两个玩家对象，并加入table中
    CPlayer* player1 = m_pPlayerMgr->CreatePlayer(notify.userid1());
    player1->m_pService = this;
    player1->m_sTableID = tableId;
    table->m_pGame->m_pPlayer1 = player1;
    CPlayer* player2 = m_pPlayerMgr->CreatePlayer(notify.userid2());
    player2->m_pService = this;
    player2->m_sTableID = tableId;
    table->m_pGame->m_pPlayer2 = player2;
    table->m_bOnline=true;
    // 使用随机数生成器来随机选择颜色
    std::random_device rd;
    std::mt19937 gen(rd());
    // 0代表黑色，1代表白色
    std::uniform_int_distribution<> dis(1, 2);
    // todo 测试用
    player1->m_cColor = (dis(gen) == 2) ? ChessColorType::Chess_Black : ChessColorType::Chess_White;
    player2->m_cColor = player1->m_cColor == ChessColorType::Chess_Black ? ChessColorType::Chess_White : ChessColorType::Chess_Black;
    
    LOG("CGameService", "OnNotifyStartGame", "player1's color = %d, player2's color = %d", player1->m_cColor, player2->m_cColor);
    player1->m_eGameState = GameState::GS_PLAYING;
    player2->m_eGameState = GameState::GS_PLAYING;
    
    table->m_pGame->OnUpdateTableStatus();
    table->m_pGame->OnGameStart();
}


void CGameService::OnREQDrawChess(MsgPackage msg){
	REQDownChess req;
	req.ParseFromString(msg.msg());
	LOG("CGameService", "OnREQDrawChess", "uid:%d, 请求下棋!",msg.user_id());
	auto player = m_pPlayerMgr->GetPlayer(msg.user_id());
	auto table = m_pTableMgr->GetTable(player->m_sTableID);
	int chessColor;
	if(player->m_cColor==Chess_White){
		chessColor=1;
	}else{
		chessColor=2;
	}
	
	int x=req.posx();
	int y=req.posy();
	LOG("CGameService", "OnREQDrawChess", "uid:%d, 棋子:%d,x:%d,y:%d",msg.user_id(), chessColor,x,y);
	table->m_iChessMatrix[x][y] = chessColor;
	std::vector<std::pair<int, int>> position = {std::make_pair(x, y), std::make_pair(x, chessColor)};
	table->m_vChessPositions.push_back(position);
	ACKDownChess DownChess;
    DownChess.set_posx(x);
	DownChess.set_posy(y);
	DownChess.set_color(static_cast<wzq::ChessColorType>(chessColor));
	if(table->m_pGame->m_pPlayer2==player){
		table->m_pGame->m_pTable->m_pService->SendSvrdMsg(table->m_pGame->m_pTable->m_pService->GetConnSvrdMsgRoute(), 
    static_cast<int64>(MESSAGE_TYPE_MY_DRAW_CHESS), 0, table->m_pGame->m_pPlayer1->m_nUserID, DownChess.SerializeAsString(), 0);
	}
	else{
		table->m_pGame->m_pTable->m_pService->SendSvrdMsg(table->m_pGame->m_pTable->m_pService->GetConnSvrdMsgRoute(), 
    static_cast<int64>(MESSAGE_TYPE_MY_DRAW_CHESS), 0, table->m_pGame->m_pPlayer2->m_nUserID, DownChess.SerializeAsString(), 0);
	}
	CheckWin(chessColor,msg,x,y);
    
    table->m_pGame->ChangePlayer();
}

void CGameService::CheckWin(int chessColor,MsgPackage msg,int x,int y)
{
	auto player = m_pPlayerMgr->GetPlayer(msg.user_id());
	auto table = m_pTableMgr->GetTable(player->m_sTableID);
    std::vector<std::pair<int, int>> winPositions;
    winPositions.push_back({x, y});
    // 检查水平方向
    int count = 1;
    for (int i = x + 1; i < 15 && table->m_iChessMatrix[i][y] == chessColor; i++) {
        count++;
        winPositions.push_back({i, y});
    }
    for (int i = x - 1; i >= 0 && table->m_iChessMatrix[i][y] == chessColor; i--) {
        count++;
        winPositions.push_back({i, y});
    }
    if (count >= 5) {
        showWinMessage(chessColor,winPositions,msg);
        return;
    }
    winPositions.clear();
    winPositions.push_back({x, y});
    // 检查垂直方向
    count = 1;
    for (int j = y + 1; j < 15 && table->m_iChessMatrix[x][j] == chessColor; j++) {
        count++;
        winPositions.push_back({x, j});
    }
    for (int j = y - 1; j >= 0 && table->m_iChessMatrix[x][j] == chessColor; j--) {
        count++;
        winPositions.push_back({x, j});
    }
    if (count >= 5) {
        showWinMessage(chessColor,winPositions,msg);
        return;
    }
    winPositions.clear();
    winPositions.push_back({x, y});
    // 检查主对角线方向
    count = 1;
    for (int i = x + 1, j = y + 1; i < 15 && j < 15 && table->m_iChessMatrix[i][j] == chessColor; i++, j++) {
        count++;
        winPositions.push_back({i, j});
    }
    for (int i = x - 1, j = y - 1; i >= 0 && j >= 0 && table->m_iChessMatrix[i][j] == chessColor; i--, j--) {
        count++;
        winPositions.push_back({i, j});
    }
    if (count >= 5) {
        showWinMessage(chessColor,winPositions,msg);
        return;
    }
    winPositions.clear();
    winPositions.push_back({x, y});
    // 检查副对角线方向
    count = 1;
    for (int i = x + 1, j = y - 1; i < 15 && j >= 0 && table->m_iChessMatrix[i][j] == chessColor; i++, j--) {
        count++;
        winPositions.push_back({i, j});
    }
    for (int i = x - 1, j = y + 1; i >= 0 && j < 15 && table->m_iChessMatrix[i][j] == chessColor; i--, j++) {
        count++;
        winPositions.push_back({i, j});
    }
    if (count >= 5) {
        showWinMessage(chessColor,winPositions,msg);
        return;
    }
}

void CGameService::showWinMessage(int chessColor,std::vector<std::pair<int, int>> winPositions,MsgPackage msg){
	GameOver gameover;
	auto player = m_pPlayerMgr->GetPlayer(msg.user_id());
	auto table = m_pTableMgr->GetTable(player->m_sTableID);
    table->m_pGame->OnGameEnd();
	LOG("CGameService", "showWinMessage", "uid:%d胜利,棋子颜色为%d!",msg.user_id(),chessColor);
	if(table->m_pGame->m_pPlayer2==player){
		player->m_eGameState=GS_WIN;
		gameover.set_state(player->m_eGameState);
		WinPosition* winPositionMsg = gameover.mutable_winposition();
        for (const auto& pos : winPositions) {
            WinPosition::Row* row = winPositionMsg->add_matrix();
            row->mutable_values()->Add(pos.first);
            row->mutable_values()->Add(pos.second);
        }
		table->m_pGame->m_pTable->m_pService->SendSvrdMsg(table->m_pGame->m_pTable->m_pService->GetConnSvrdMsgRoute(), 
    	static_cast<int64>(MESSAGE_TYPE_GAME_OVER), 0, table->m_pGame->m_pPlayer2->m_nUserID, gameover.SerializeAsString(), 0);

		table->m_pGame->m_pPlayer1->m_eGameState=GS_LOSE;
		gameover.set_state(table->m_pGame->m_pPlayer1->m_eGameState);
		table->m_pGame->m_pTable->m_pService->SendSvrdMsg(table->m_pGame->m_pTable->m_pService->GetConnSvrdMsgRoute(), 
    	static_cast<int64>(MESSAGE_TYPE_GAME_OVER), 0, table->m_pGame->m_pPlayer1->m_nUserID, gameover.SerializeAsString(), 0);
	}
	else{
		player->m_eGameState=GS_WIN;
		gameover.set_state(player->m_eGameState);
		WinPosition* winPositionMsg = gameover.mutable_winposition();
        for (const auto& pos : winPositions) {
            WinPosition::Row* row = winPositionMsg->add_matrix();
            row->mutable_values()->Add(pos.first);
            row->mutable_values()->Add(pos.second);
        }
		table->m_pGame->m_pTable->m_pService->SendSvrdMsg(table->m_pGame->m_pTable->m_pService->GetConnSvrdMsgRoute(), 
    	static_cast<int64>(MESSAGE_TYPE_GAME_OVER), 0, table->m_pGame->m_pPlayer1->m_nUserID, gameover.SerializeAsString(), 0);

		table->m_pGame->m_pPlayer2->m_eGameState=GS_LOSE;
		gameover.set_state(table->m_pGame->m_pPlayer2->m_eGameState);
		table->m_pGame->m_pTable->m_pService->SendSvrdMsg(table->m_pGame->m_pTable->m_pService->GetConnSvrdMsgRoute(), 
    	static_cast<int64>(MESSAGE_TYPE_GAME_OVER), 0, table->m_pGame->m_pPlayer2->m_nUserID, gameover.SerializeAsString(), 0);
	}
	
	if(table->m_bOnline){
	PlayingResult result;
	result.set_gameresult(player->m_eGameState);
	result.set_winuserid(player->m_nUserID);
	result.set_color(player->m_cColor);
// 遍历 m_vChessPositions 中的每个棋子位置信息
	for (const auto& position : table->m_vChessPositions) {
		ChessMove* move = result.add_moves();
		move->set_x(position[0].first); // 存储 x 值
		move->set_y(position[0].second); // 存储 y 值
	}
	if(player==table->m_pGame->m_pPlayer2){
		result.set_loseuserid(table->m_pGame->m_pPlayer1->m_nUserID);
	}
	else{
		result.set_loseuserid(table->m_pGame->m_pPlayer2->m_nUserID);
	}
	table->m_pGame->m_pTable->m_pService->SendSvrdMsg(table->m_pGame->m_pTable->m_pService->GetAuthSvrdMsgRoute(), 
    	static_cast<int64>(MESSAGE_TYPE_NOTIFY_GAME_SETTLEMENT), 0, player->m_nUserID, result.SerializeAsString(), 0);
	}
	

	for (int i = 0; i < 15; ++i) {
        for (int j = 0; j < 15; ++j) {
				table->m_iChessMatrix[i][j] = 0;}
	}
	table->m_vChessPositions.clear();
}

void CGameService::OnREQRegretChess(MsgPackage msg){
	LOG("CGameService", "OnREQRegretChess", "uid:%d,请求悔棋",msg.user_id());
	ACKRegretChess ack;
	ack.set_resultcode(RESULT_CODE_SUCCESS);
	auto player = m_pPlayerMgr->GetPlayer(msg.user_id());
	auto table = m_pTableMgr->GetTable(player->m_sTableID);
	if(player==table->m_pGame->m_pPlayer1){
		table->m_pGame->m_pTable->m_pService->SendSvrdMsg(table->m_pGame->m_pTable->m_pService->GetConnSvrdMsgRoute(), 
    	static_cast<int64>(MESSAGE_TYPE_ASK_REGRET_CHESS | MESSAGE_TYPE_ACK), 0, table->m_pGame->m_pPlayer2->m_nUserID, ack.SerializeAsString(), 0);
	}
	else{
		table->m_pGame->m_pTable->m_pService->SendSvrdMsg(table->m_pGame->m_pTable->m_pService->GetConnSvrdMsgRoute(), 
    	static_cast<int64>(MESSAGE_TYPE_ASK_REGRET_CHESS | MESSAGE_TYPE_ACK), 0, table->m_pGame->m_pPlayer1->m_nUserID, ack.SerializeAsString(), 0);
	}
}

void CGameService::OnREQAgreeRegretChess(MsgPackage msg){
	LOG("CGameService", "OnREQAgreeRegretChess", "uid:%d,同意悔棋",msg.user_id());
	ACKAgreeRegretChess ack;
	ack.set_resultcode(RESULT_CODE_SUCCESS);
	auto player = m_pPlayerMgr->GetPlayer(msg.user_id());
	auto table = m_pTableMgr->GetTable(player->m_sTableID);
	std::vector<std::pair<int, int>>& lastVector = table->m_vChessPositions.back();
	std::pair<int, int>& lastPosition = lastVector.back();

        // 获取最后一个元素的x和y值
    int lastX = lastPosition.first;
    int lastY = lastPosition.second;
    // 获取最后一个元素的x和y值
	table->m_vChessPositions.pop_back();
	table->m_iChessMatrix[lastX][lastY]=0;
	if(player==table->m_pGame->m_pPlayer1){
		table->m_pGame->m_pTable->m_pService->SendSvrdMsg(table->m_pGame->m_pTable->m_pService->GetConnSvrdMsgRoute(), 
    	static_cast<int64>(MESSAGE_TYPE_NOTIFY_AGREE_REGRET_CHESS | MESSAGE_TYPE_ACK), 0, table->m_pGame->m_pPlayer2->m_nUserID, ack.SerializeAsString(), 0);
	}
	else{
		table->m_pGame->m_pTable->m_pService->SendSvrdMsg(table->m_pGame->m_pTable->m_pService->GetConnSvrdMsgRoute(), 
    	static_cast<int64>(MESSAGE_TYPE_NOTIFY_AGREE_REGRET_CHESS | MESSAGE_TYPE_ACK), 0, table->m_pGame->m_pPlayer1->m_nUserID, ack.SerializeAsString(), 0);
	}
}


void CGameService::OnREQDisagreeRegretChess(MsgPackage msg){
	LOG("CGameService", "OnREQDisagreeRegretChess", "uid:%d,不同意悔棋",msg.user_id());
	ACKAgreeRegretChess ack;
	auto player = m_pPlayerMgr->GetPlayer(msg.user_id());
	auto table = m_pTableMgr->GetTable(player->m_sTableID);
	ack.set_resultcode(RESULT_CODE_SUCCESS);
	if(player==table->m_pGame->m_pPlayer1){
		table->m_pGame->m_pTable->m_pService->SendSvrdMsg(table->m_pGame->m_pTable->m_pService->GetConnSvrdMsgRoute(), 
    	static_cast<int64>(MESSAGE_TYPE_NOTIFY_DISAGREE_REGRET_CHESS | MESSAGE_TYPE_ACK), 0, table->m_pGame->m_pPlayer2->m_nUserID, ack.SerializeAsString(), 0);
	}
	else{
		table->m_pGame->m_pTable->m_pService->SendSvrdMsg(table->m_pGame->m_pTable->m_pService->GetConnSvrdMsgRoute(), 
    	static_cast<int64>(MESSAGE_TYPE_NOTIFY_DISAGREE_REGRET_CHESS | MESSAGE_TYPE_ACK), 0, table->m_pGame->m_pPlayer1->m_nUserID, ack.SerializeAsString(), 0);
	}
}

void CGameService::OnNotifyGiveUp(MsgPackage msg){
	LOG("CGameService", "OnNotifyGiveUp", "uid:%d认输",msg.user_id());
	GameOver gameover;
	auto player = m_pPlayerMgr->GetPlayer(msg.user_id());
	auto table = m_pTableMgr->GetTable(player->m_sTableID);
    table->m_pGame->OnGameEnd();
	player->m_eGameState=GS_LOSE;
	if(table->m_pGame->m_pPlayer2==player){
		gameover.set_state(player->m_eGameState);
		table->m_pGame->m_pTable->m_pService->SendSvrdMsg(table->m_pGame->m_pTable->m_pService->GetConnSvrdMsgRoute(), 
    	static_cast<int64>(MESSAGE_TYPE_GAME_OVER), 0, table->m_pGame->m_pPlayer2->m_nUserID, gameover.SerializeAsString(), 0);

		table->m_pGame->m_pPlayer1->m_eGameState=GS_WIN;
		gameover.set_state(table->m_pGame->m_pPlayer1->m_eGameState);
		table->m_pGame->m_pTable->m_pService->SendSvrdMsg(table->m_pGame->m_pTable->m_pService->GetConnSvrdMsgRoute(), 
    	static_cast<int64>(MESSAGE_TYPE_GAME_OVER), 0, table->m_pGame->m_pPlayer1->m_nUserID, gameover.SerializeAsString(), 0);
	}
	else{
		gameover.set_state(player->m_eGameState);
		table->m_pGame->m_pTable->m_pService->SendSvrdMsg(table->m_pGame->m_pTable->m_pService->GetConnSvrdMsgRoute(), 
    	static_cast<int64>(MESSAGE_TYPE_GAME_OVER), 0, table->m_pGame->m_pPlayer1->m_nUserID, gameover.SerializeAsString(), 0);

		table->m_pGame->m_pPlayer2->m_eGameState=GS_WIN;
		gameover.set_state(table->m_pGame->m_pPlayer2->m_eGameState);
		table->m_pGame->m_pTable->m_pService->SendSvrdMsg(table->m_pGame->m_pTable->m_pService->GetConnSvrdMsgRoute(), 
    	static_cast<int64>(MESSAGE_TYPE_GAME_OVER), 0, table->m_pGame->m_pPlayer2->m_nUserID, gameover.SerializeAsString(), 0);
	}

	if(table->m_bOnline){
		PlayingResult result;
		result.set_gameresult(player->m_eGameState);
		result.set_loseuserid(player->m_nUserID);
		
	// 遍历 m_vChessPositions 中的每个棋子位置信息
		for (const auto& position : table->m_vChessPositions) {
			ChessMove* move = result.add_moves();
			move->set_x(position[0].first); // 存储 x 值
			move->set_y(position[0].second); // 存储 y 值
		}
		if(player==table->m_pGame->m_pPlayer2){
			result.set_winuserid(table->m_pGame->m_pPlayer1->m_nUserID);
			result.set_color(table->m_pGame->m_pPlayer1->m_cColor);
		}
		else{
			result.set_winuserid(table->m_pGame->m_pPlayer2->m_nUserID);
			result.set_color(table->m_pGame->m_pPlayer2->m_cColor);
		}
		table->m_pGame->m_pTable->m_pService->SendSvrdMsg(table->m_pGame->m_pTable->m_pService->GetAuthSvrdMsgRoute(), 
			static_cast<int64>(MESSAGE_TYPE_NOTIFY_GAME_SETTLEMENT), 0, player->m_nUserID, result.SerializeAsString(), 0);
	}
	

	for (int i = 0; i < 15; ++i) {
        for (int j = 0; j < 15; ++j) {
				table->m_iChessMatrix[i][j] = 0;}
	}
	table->m_vChessPositions.clear();
}

void CGameService::OnNotifyPeaceChess(MsgPackage msg)
{
	LOG("CGameService", "OnNotifyPeaceChess", "uid:%d,请求和棋",msg.user_id());
	NotifyASKPeaceChess ack;
	auto player = m_pPlayerMgr->GetPlayer(msg.user_id());
	auto table = m_pTableMgr->GetTable(player->m_sTableID);
	if(player==table->m_pGame->m_pPlayer1){
		table->m_pGame->m_pTable->m_pService->SendSvrdMsg(table->m_pGame->m_pTable->m_pService->GetConnSvrdMsgRoute(), 
    	static_cast<int64>(MESSAGE_TYPE_NOTIFY_ASK_PEACE_CHESS), 0, table->m_pGame->m_pPlayer2->m_nUserID, ack.SerializeAsString(), 0);
	}
	else{
		table->m_pGame->m_pTable->m_pService->SendSvrdMsg(table->m_pGame->m_pTable->m_pService->GetConnSvrdMsgRoute(), 
    	static_cast<int64>(MESSAGE_TYPE_NOTIFY_ASK_PEACE_CHESS), 0, table->m_pGame->m_pPlayer1->m_nUserID, ack.SerializeAsString(), 0);
	}
}

void CGameService::OnNotifyAgreePeaceChess(MsgPackage msg)
{
	LOG("CGameService", "OnNotifyAgreePeaceChess", "uid:%d同意和棋",msg.user_id());
	GameOver gameover;
	auto player = m_pPlayerMgr->GetPlayer(msg.user_id());
	auto table = m_pTableMgr->GetTable(player->m_sTableID);
    table->m_pGame->OnGameEnd();
	table->m_pGame->m_pPlayer1->m_eGameState=GS_DRAW;
	table->m_pGame->m_pPlayer2->m_eGameState=GS_DRAW;
	gameover.set_state(table->m_pGame->m_pPlayer2->m_eGameState);
	table->m_pGame->m_pTable->m_pService->SendSvrdMsg(table->m_pGame->m_pTable->m_pService->GetConnSvrdMsgRoute(), 
    static_cast<int64>(MESSAGE_TYPE_GAME_OVER), 0, table->m_pGame->m_pPlayer2->m_nUserID, gameover.SerializeAsString(), 0);
	gameover.set_state(table->m_pGame->m_pPlayer1->m_eGameState);
	table->m_pGame->m_pTable->m_pService->SendSvrdMsg(table->m_pGame->m_pTable->m_pService->GetConnSvrdMsgRoute(), 
    static_cast<int64>(MESSAGE_TYPE_GAME_OVER), 0, table->m_pGame->m_pPlayer1->m_nUserID, gameover.SerializeAsString(), 0);
	

	if(table->m_bOnline){
	PlayingResult result;
	result.set_gameresult(player->m_eGameState);
	result.set_loseuserid(player->m_nUserID);
	
// 遍历 m_vChessPositions 中的每个棋子位置信息
	for (const auto& position : table->m_vChessPositions) {
		ChessMove* move = result.add_moves();
		move->set_x(position[0].first); // 存储 x 值
		move->set_y(position[0].second); // 存储 y 值
	}
	if(player==table->m_pGame->m_pPlayer2){
		result.set_winuserid(table->m_pGame->m_pPlayer1->m_nUserID);
		result.set_color(table->m_pGame->m_pPlayer1->m_cColor);
	}
	else{
		result.set_winuserid(table->m_pGame->m_pPlayer2->m_nUserID);
		result.set_color(table->m_pGame->m_pPlayer2->m_cColor);
	}
	table->m_pGame->m_pTable->m_pService->SendSvrdMsg(table->m_pGame->m_pTable->m_pService->GetAuthSvrdMsgRoute(), 
		static_cast<int64>(MESSAGE_TYPE_NOTIFY_GAME_SETTLEMENT), 0, player->m_nUserID, result.SerializeAsString(), 0);
	}

	for (int i = 0; i < 15; ++i) {
        for (int j = 0; j < 15; ++j) {
				table->m_iChessMatrix[i][j] = 0;}
	}
	table->m_vChessPositions.clear();
}

void CGameService::OnNotifyDisagreePeaceChess(MsgPackage msg)
{
	LOG("CGameService", "OnNotifyDisagreePeaceChess", "uid:%d不同意和棋",msg.user_id());
	ACKDisagreePeaceChess ack;
	auto player = m_pPlayerMgr->GetPlayer(msg.user_id());
	auto table = m_pTableMgr->GetTable(player->m_sTableID);
	if(player==table->m_pGame->m_pPlayer1){
	table->m_pGame->m_pTable->m_pService->SendSvrdMsg(table->m_pGame->m_pTable->m_pService->GetConnSvrdMsgRoute(), 
    static_cast<int64>(MESSAGE_TYPE_NOTIFY_ASK_DISAGREE_PEACE_CHESS), 0, table->m_pGame->m_pPlayer2->m_nUserID, ack.SerializeAsString(), 0);
	}
	else{
		table->m_pGame->m_pTable->m_pService->SendSvrdMsg(table->m_pGame->m_pTable->m_pService->GetConnSvrdMsgRoute(), 
    static_cast<int64>(MESSAGE_TYPE_NOTIFY_ASK_DISAGREE_PEACE_CHESS), 0, table->m_pGame->m_pPlayer1->m_nUserID, ack.SerializeAsString(), 0);
	}
}

bool CGameService::OnReceiveQueueMsg(BYTE* pbyBuffer, int nBufferLen)
{
 	auto& client = StatsdClientSingleton::getInstance();
    client.increment("GameServer.req.all.count");

	MsgPackage msg ;
    if(! msg.ParseFromArray(pbyBuffer, nBufferLen))
    {
        LOG("CGameService", "OnReceiveQueueMsg", "parse failed!", msg.msg_id());
        return false ;
    }

    LOG("CGameService", "OnReceiveQueueMsg", "收到一条消息:%d!\n", msg.msg_id());
	switch (msg.msg_id())
	{
	case MESSAGE_TYPE_CREATE_TABLE | MESSAGE_TYPE_REQ:
		OnREQCreateTable(msg);
		break;
	case MESSAGE_TYPE_JOIN_TABLE | MESSAGE_TYPE_REQ:
		OnREQJoinTable(msg);
		break;
	case MESSAGE_TYPE_READY_GAME | MESSAGE_TYPE_REQ:
		OnREQReadyGame(msg);
		break;
	case MESSAGE_TYPE_START_GAME | MESSAGE_TYPE_REQ:
		OnREQStartGame(msg);
		break;

    case MESSAGE_TYPE_NOTIFY_START_GAME:
        OnNotifyStartGame(msg);
        break;

	case MESSAGE_TYPE_DRAW_CHESS | MESSAGE_TYPE_REQ:
		OnREQDrawChess(msg);
		break;
	case MESSAGE_TYPE_REGRET_CHESS | MESSAGE_TYPE_REQ:
		OnREQRegretChess(msg);
		break;
	case MESSAGE_TYPE_AGREE_REGRET_CHESS | MESSAGE_TYPE_REQ:
		OnREQAgreeRegretChess(msg);
		break;
	case MESSAGE_TYPE_DISAGREE_REGRET_CHESS | MESSAGE_TYPE_REQ:
		OnREQDisagreeRegretChess(msg);
		break;
	case MESSAGE_TYPE_NOTIFY_GIVE_UP:
		OnNotifyGiveUp(msg);
		break;
	case MESSAGE_TYPE_NOTIFY_PEACE_CHESS:
		OnNotifyPeaceChess(msg);
		break;
	case MESSAGE_TYPE_NOTIFY_AGREE_PEACE_CHESS:
		OnNotifyAgreePeaceChess(msg);
		break;
	case MESSAGE_TYPE_NOTIFY_DISAGREE_PEACE_CHESS:
		OnNotifyDisagreePeaceChess(msg);
		break;
	default:
		break;

	}

	// 服务器请求打点
    std::string statKey = StatsUtils::GetStatKeyFromMsgID(msg.msg_id());
    client.increment(statKey);

    return true;
}

void* CGameService::RecvProc(void* pParam)
{
    CGameService* pService = (CGameService*)(pParam);
    if(! pService) return nullptr ;
    
    while (true)
    {
		std::string key = "WZQ_MSG_QUEUE_GAMESVRD_wzqgame_01";
		std::string strBuffer;
		int nResult = pService ->mqr() ->list_blpop(key, strBuffer , 1);
		if(IsNotZero(nResult))
		{
			continue;
		}
		pService ->AddEventQueueMsg(strBuffer);
    }
    return nullptr ;
}

bool CGameService::InitRecvMsgQueueThread()
{
    pthread_t nThreadID ;
    int ret = pthread_create(&nThreadID,NULL,CGameService::RecvProc,this);
    return ret == 0 ;
}

//获取连接服的路由信息
std::string CGameService::GetConnSvrdMsgRoute()
{
    return "WZQ_MSG_QUEUE_CONNSVRD_conn101_01";
}

std::string CGameService::GetAuthSvrdMsgRoute()
{
    return "WZQ_MSG_QUEUE_AUTHSVRD_wzqauth_01";
}

int CGameService::SendSvrdMsg(const std::string& msgroute , int64 msg_id , int64 seq_id , int64 user_id , const std::string& msg,int64 nClientID)
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

string CGameService::GenUserRawDataKey(int64 userID) {
    char key[50];
    sprintf(key, "USER_RAW_DATA_%d", userID);
    return string(key);
}

UserRawData CGameService::GetBaseInfo(int64 userID) {
    LOG("CGameService", "GetBaseInfo", "userID = %llu",userID);
	string displayID, name;
	string key = GenUserRawDataKey(userID);
	cache()->hash_get(key, "displayID", displayID);
	cache()->hash_get(key, "name", name);
	UserRawData baseInfo;
	baseInfo.set_userid(userID);
	baseInfo.set_displayid(displayID);
	baseInfo.set_name(name);
	LOG("CGameService", "GetBaseInfo", "获取userID:%d基本信息, name:%s, displayID:%s",userID, name.c_str(), displayID.c_str());
	return baseInfo;
}
