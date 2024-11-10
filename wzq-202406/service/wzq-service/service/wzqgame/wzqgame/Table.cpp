#include "Table.h"
#include "WZQGame.h"
#include "mcgevent.h"
#include "globalfunc.h"
#include "log.h"
#include "PlayerMgr.h"
#include "ProtocolDef.h"
#include "WZQGame.h"
#include "Player.h"

extern CService *g_pService;
class CGameService;

CTable::CTable(std::string tableID)
{
	m_pGame = new CWZQGame();
	m_sTableID = tableID;
	m_hFixTimer = NULL;
	m_pTableMaster = NULL;
	for (int i = 0; i < 15; ++i) {
        for (int j = 0; j < 15; ++j) {
				m_iChessMatrix[i][j] = 0;}
	}
}

CTable::~CTable()
{
	if (IsNotNULL(m_hFixTimer))
	{
		CMcgTimer::DeleteTimer(m_hFixTimer, true);
	}

	if (IsNotNULL(m_pGame))
	{
		delete m_pGame;
	}
}

bool CTable::Init()
{
    mcgWriteLog("CTable::Init  \n");
	m_pService=(CGameService *)g_pService;
	m_pGame->m_pTable = this;
	m_hFixTimer = CMcgTimer::CreateTimer((LPTHREAD_START_ROUTINE)PTimerOut, this, 1000, 1000);
	return true;
}

DWORD CTable::OnPTimerOut()
{
	if (IsNotNULL(m_pGame))
	{
		m_pGame->OnTimer();
	}

	return 0;
}

int CTable::OnClientMessage(int64 user_id, DWORD dwMsgID, DWORD dwSeqID, const byte* pBuffer, int nLen)
{
	return m_pGame->OnClientMessage(user_id,dwMsgID,dwSeqID,pBuffer,nLen);
}

string CTable::GetTableID()
{
	return m_sTableID;
}

bool CTable::SendClientMsg(DWORD dwMsgID, DWORD dwSeqID, PROTOBUF_MESSAGE* pMsg)
{
    return false;
}

bool CTable::SendClientMsgTo(int64 user_id, DWORD dwMsgID, DWORD dwSeqID, PROTOBUF_MESSAGE* pMsg)
{
	CPlayer* pPlayer = m_pService->GetPlayerByID(user_id);
	if (IsNULL(pPlayer))
	{
		mcgWriteLog("Error Table(ID = %s)::SendClientMsgTo,not exists the player %lld", m_sTableID.c_str(), user_id);
		return false;
	}
	return pPlayer->SendMsg(dwMsgID,pMsg,dwSeqID);
}


