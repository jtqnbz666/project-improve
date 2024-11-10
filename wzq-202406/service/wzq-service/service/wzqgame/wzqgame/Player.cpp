#include "Player.h"
#include "Table.h"
#include "mcgevent.h"
#include "log.h"
#include "TableMgr.h"
#include "PlayerMgr.h"
#include "ProtocolDef.h"

#include "SeqUtil.h"

extern CService *g_pService;

class CGameService;

CPlayer::CPlayer(int64 user_id)
{
	m_sTableID = "";
	m_nUserID = user_id;
	m_pService = (CGameService *)g_pService;
	//m_bIsReady = 0;
}

CPlayer::~CPlayer()
{
}

bool CPlayer::Init()
{
	return true;
}

void CPlayer::Disconnect()
{
	mcgWriteLog("Info Player(%p)(ID = %lld, tableID=%s)::Disconnect, disconnect begin", this, m_nUserID, m_sTableID.c_str());
    mcgWriteLog("Info Player(ID = %lld)::Disconnect, disconnect end", m_nUserID);
}

string CPlayer::GetTableID()
{
	return m_sTableID;
}


bool CPlayer::SendMsg(DWORD dwMsgID, PROTOBUF_MESSAGE* pMsg, DWORD dwSeqID)
{
    return true;
}


bool CPlayer::OnMsg(DWORD dwMsgID, DWORD dwSeqID, const byte* pBuffer, int nLen)
{
	bool bResult = true;
	bool bHandled = true;
	switch(dwMsgID)
	{
	default:
		bHandled = false;
		break;
	}
	if (!bHandled)
	{
		CTable* pTable = m_pService->GetTableByID(m_sTableID);
		if (IsNotNULL(pTable))
		{
			int nRet = pTable->OnClientMessage(m_nUserID, dwMsgID,dwSeqID,pBuffer,nLen);
			if (nRet != 0)
			{
				bResult = false;
			}
		}
	}
	
	return bResult;
}














