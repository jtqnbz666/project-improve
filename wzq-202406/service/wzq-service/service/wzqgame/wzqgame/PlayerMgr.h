#ifndef __PLAYER_MGR_H__
#define __PLAYER_MGR_H__

#include <map>
#include <google/protobuf/stubs/common.h>
using google::protobuf::int32;
using google::protobuf::int64;
using std::map;

class CGameService;
class CPlayer;

class CPlayerMgr
{
public:
	CPlayerMgr();
	virtual ~CPlayerMgr();
public:	
	void RemovePlayer(int64 user_id);										//
	CPlayer* GetPlayer(int64 user_id);										//
	int Size();											//
    CPlayer* CreatePlayer(int64 user_id);                                   //获取一个玩家实例
public:
	map<int64, CPlayer*> m_playersMap;
	CGameService* m_pService;
};

#endif
