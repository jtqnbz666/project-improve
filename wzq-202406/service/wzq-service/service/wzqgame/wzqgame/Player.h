#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "GameService.h"
#include "proto/wzqGame.pb.h"
class CGameClient;
class CGameService;

class CPlayer
{
public:
	CPlayer(int64 user_id);
	virtual ~CPlayer();
public:
	bool Init();
	void Disconnect();
	string GetTableID();
public:
	bool SendMsg(DWORD dwMsgID, PROTOBUF_MESSAGE* pMsg, DWORD dwSeqID);
	bool OnMsg(DWORD dwMsgID, DWORD dwSeqID, const byte* pBuffer, int nLen);
public:
	CGameService* m_pService;
	int64 m_nUserID;
	string m_sTableID;				// 桌子id

	bool m_bIsReady;				// 是否准备

	GameState m_eGameState;         // 游戏状态
	ChessColorType m_cColor;		// 棋子颜色
	int posx;
	int posy;
};

#endif
