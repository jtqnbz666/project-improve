#include "PlayerMgr.h"
#include "ConnService.h"
#include "ConnClient.h"
#include "Player.h"
#include "mcgevent.h"
#include "log.h"
#include "AppDef.h"
#include <stdio.h>

extern CService *g_pService;

CPlayerMgr::CPlayerMgr()
{
	m_pService = (CConnService*)g_pService;
}

CPlayerMgr::~CPlayerMgr()
{
}

int CPlayerMgr::Size()
{
	return (int)m_playersMap.size();
}

CPlayer* CPlayerMgr::CreatePlayer(int64 userID, const char* skey, int* error_code)
{
	CPlayer* pPlayer = NULL;
	bool isNew = true;
	map<int64,CPlayer*>::iterator iter = m_playersMap.find(userID);
	if (iter != m_playersMap.end())
	{
		pPlayer = m_playersMap[userID];
		isNew = false;
	}
	else
	{	
		pPlayer = new CPlayer(userID);
		if (!pPlayer->Init())
		{
			delete pPlayer;
			return NULL;
		}
		m_playersMap[userID] = pPlayer;
	}

	LOG("CPlayerMgr", "CreatePlayer", "创建Player对象: %d, isNew:%d", userID, isNew);

	return pPlayer;
}

void CPlayerMgr::RemovePlayer(int64 user_id)
{
	map<int64,CPlayer*>::iterator iter = m_playersMap.find(user_id);
	if (iter != m_playersMap.end())
	{
        if(iter ->second) delete iter ->second;
        
		m_playersMap.erase(iter);
	}
}

CPlayer* CPlayerMgr::GetPlayer(int64 user_id)
{
	CPlayer* player = NULL;
	map<int64,CPlayer*>::iterator iter = m_playersMap.find(user_id);
	if (iter != m_playersMap.end())
	{
		player = m_playersMap[user_id];
	}

	return player;
}
