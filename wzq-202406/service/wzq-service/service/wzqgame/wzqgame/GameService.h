#ifndef __GAME_SERVICE_H__
#define __GAME_SERVICE_H__

#include "service.h"
#include "RedisIntf.h"
#include "AppDef.h"
#include <string>
#include <list>
#include <map>
#include <set>
#include "TableMgr.h"
#include "PlayerMgr.h"
#include "log.h"

using namespace std;

class CGameService : public CService
{
public:
	CGameService();
	virtual ~CGameService();
public:
	virtual bool OnInit();									
	virtual bool OnTimeOut(DWORD dwEventID);				
	virtual void OnExitProcess(UINT uExitCode);				
    virtual bool OnReceiveQueueMsg(BYTE* pbyBuffer, int nBufferLen);
public:
	CPlayer* GetPlayerByID(int64 user_id);
	CTable* GetTableByID(string tableID);
	int GetTotalPlayer();
	int GetTotalTable();
    bool InitSvrdID();
    std::string GetSvrdID(){return m_strSvrdID;}
	std::string GetConnSvrdMsgRoute();

    std::string GetAuthSvrdMsgRoute();




    UserRawData GetBaseInfo(int64 userID);
    string GenUserRawDataKey(int64 userID);

	//向内部服务转发消息
    int SendSvrdMsg(const std::string& msgroute , int64 msg_id , int64 seq_id , int64 user_id , const std::string& msg,int64 nClientID);
    
private:
	bool InitAppLog();
	bool InitRedis();
    
    //初始化消息队列缓存
    bool InitMQRedis();
private:
    static void* RecvProc(void* pParam);
    
    bool InitRecvMsgQueueThread();
public:
	void UpLogLevel();			
	void DownLogLevel();		
    void OnSvrdClose();
    bool SvrdNeedClose();
    

public:
	void OnREQJoinTable(MsgPackage msg);
	void OnREQCreateTable(MsgPackage msg);
	void OnREQReadyGame(MsgPackage msg);
	void OnREQStartGame(MsgPackage msg);
    void OnNotifyStartGame(MsgPackage msg);
	void OnREQDrawChess(MsgPackage msg);
	void OnREQRegretChess(MsgPackage msg);
	void OnREQAgreeRegretChess(MsgPackage msg);
	void OnREQDisagreeRegretChess(MsgPackage msg);
	void OnNotifyGiveUp(MsgPackage msg);
	void OnNotifyPeaceChess(MsgPackage msg);
	void OnNotifyAgreePeaceChess(MsgPackage msg);
	void OnNotifyDisagreePeaceChess(MsgPackage msg);
public:
	void CheckWin(int chessColor,MsgPackage msg,int x,int y);
	void showWinMessage(int chessColor,std::vector<std::pair<int, int>> winPositions,MsgPackage msg);
public:
	CTableMgr* m_pTableMgr;
	CPlayerMgr* m_pPlayerMgr;
	CRedisIntf* m_pRedis;
	CLog* m_pLogicLog;
    
    //消息队列缓存
    CRedisIntf*               m_pMQRedis;
private:
	HANDLE  m_hTrySendTimer;
	bool    m_bInitOk;											//
	string  m_strAppLogPath;
	string  m_strAppServiceName;
    bool    m_bSvrdClose        ;                             //服务器收到关闭的指令 ,即将关闭
    std::string m_strSvrdID    ;                             //服务器ID
	
};

#endif
