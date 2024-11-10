#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "ConnService.h"
#include <list>
#include "./proto/wzqProtocol.pb.h"
#include "./proto/wzqGame.pb.h"
#include "AppDef.h"

using namespace wzq;
class CConnClient ;

class CPlayer
{
public:
	CPlayer(int64 user_id);
	virtual ~CPlayer();
public:
	bool Init();
	void Disconnect(bool needNtf = true);
public:
	bool SendMsg(DWORD dwMsgID, PROTOBUF_MESSAGE* pMsg, DWORD dwSeqID);
    bool SendMsg(DWORD dwMsgID, const std::string& strBuffer, DWORD dwSeqID);
    
	bool OnMsg(DWORD dwMsgID, DWORD dwSeqID, const byte* pBuffer, int nLen);

public:
	bool OnREQCreateTable(DWORD dwSeqID, std::string& pBuffer, int nLen);
	bool OnREQJoinTable(DWORD dwSeqID, std::string& pBuffer, int nLen);
	bool OnREQReadyGame(DWORD dwSeqID, std::string& pBuffer, int nLen);
	bool OnREQStartGame(DWORD dwSeqID, std::string& pBuffer, int nLen);
    bool OnREQMatchGame(DWORD dwSeqID, std::string& pBuffer, int nLen);
    bool OnNotifyCancelMatch(DWORD dwSeqID, std::string& pBuffer, int nLen);
	bool OnREQChangeName(DWORD dwSeqID, std::string& pBuffer, int nLen);
	bool OnDrawChess(DWORD dwSeqID, std::string& pBuffer, int nLen);
	bool OnRegretChess(DWORD dwSeqID, std::string& pBuffer, int nLen);
	bool OnREQAgreeRegretChess(DWORD dwSeqID, std::string& pBuffer, int nLen);
	bool OnREQDisagreeRegretChess(DWORD dwSeqID, std::string& pBuffer, int nLen);
	bool OnNotifyGiveUp(DWORD dwSeqID, std::string& pBuffer, int nLen);
	bool OnNotifyPeaceChess(DWORD dwSeqID, std::string& pBuffer, int nLen);
	bool OnNotifyAgreePeaceChess(DWORD dwSeqID, std::string& pBuffer, int nLen);
	bool OnNotifyDisagreePeaceChess(DWORD dwSeqID, std::string& pBuffer, int nLen);
	bool OnREQLeaderBoardInfo(DWORD dwSeqID, std::string& pBuffer, int nLen);

private:
    //保持连接的消息
    bool OnHold(DWORD dwSeqID, const byte* pBuffer, int nLen);
public:
	CConnClient*                            m_pClient;
	CConnService*                           m_pService;
	int64                                   m_nUserID;
};

#endif
