#ifndef __TABLE_H__
#define __TABLE_H__

#include "GameService.h"
#include "mcgtimer.h"
#include <string>

class CWZQGame;
class CGameService;

class CTable
{
public:
	CTable(std::string tableID);
	~CTable();
public:
	bool Init();
	static LPTHREAD_START_ROUTINE PTimerOut( LPVOID arg ) {((CTable *)arg)->OnPTimerOut();return 0;}
	DWORD OnPTimerOut();
public:
	int OnClientMessage(int64 user_id, DWORD dwMsgID, DWORD dwSeqID, const byte* pBuffer, int nLen);
public:
/*************************************************************************************************************/
	virtual std::string GetTableID();
	virtual bool SendClientMsg(DWORD dwMsgID, DWORD dwSeqID, PROTOBUF_MESSAGE* pMsg);
	virtual bool SendClientMsgTo(int64 user_id, DWORD dwMsgID, DWORD dwSeqID, PROTOBUF_MESSAGE* pMsg);

	CPlayer* m_pTableMaster;			// 桌主
	HANDLE m_hFixTimer;
	CGameService* m_pService;
    CWZQGame* m_pGame;
	std::string m_sTableID;
	int m_iChessMatrix[15][15]; 						// 初始化为0，表示没有棋子
	std::vector<std::vector<std::pair<int, int>>> m_vChessPositions;//保存下棋顺序
	bool m_bOnline=false;
};

#endif
