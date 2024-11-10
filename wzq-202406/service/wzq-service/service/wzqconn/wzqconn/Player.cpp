#include "Player.h"
#include "mcgevent.h"
#include "log.h"
#include "PlayerMgr.h"
#include "ProtocolDef.h"
#include "ConnClient.h"
#include "SeqUtil.h"

extern CService *g_pService;

class CGameService;

CPlayer::CPlayer(int64 user_id)
{
	m_pClient = NULL;
	m_nUserID = user_id;
	m_pService = (CConnService *)g_pService;
}

CPlayer::~CPlayer()
{
}

bool CPlayer::Init()
{
	return true;
}

void CPlayer::Disconnect(bool needNtf)
{
    NotifyCancelMatch notify;
    notify.set_userid(m_nUserID);
    
    LOG("CPlayer", "Disconnect", "玩家:%d通知取消匹配", m_nUserID);
    m_pService->SendSvrdMsg(m_pService->GetRoomSvrdMsgRoute(), MESSAGE_TYPE_CANCEL_MATCH, 0, m_nUserID, notify.SerializeAsString(), m_pService->GetClientID());
    return ;
}

bool CPlayer::SendMsg(DWORD dwMsgID, const std::string& strBuffer, DWORD dwSeqID)
{
    if (IsNULL(m_pClient))
	{
		mcgWriteLog("Error,Player(ID = %lld)::SendMsg, m_pClient is NULL", m_nUserID);
		return false;
	}
    
	return m_pClient->SendMsg(dwMsgID, strBuffer, dwSeqID);
}

bool CPlayer::SendMsg(DWORD dwMsgID, PROTOBUF_MESSAGE* pMsg, DWORD dwSeqID)
{
	if (IsNULL(m_pClient))
	{
		mcgWriteLog("Error,Player(ID = %lld)::SendMsg, m_pClient is NULL", m_nUserID);
		return false;
	}

	return m_pClient->SendMsg(dwMsgID, pMsg, dwSeqID);
}

bool CPlayer::OnMsg(DWORD dwMsgID, DWORD dwSeqID, const byte* pBuffer, int nLen)
{
	LOG("CPlayer", "OnMsg", "收到消息:%s", to_string(dwMsgID).c_str());

	size_t length = strlen(reinterpret_cast<const char*>(pBuffer)); 
	std::string strBuffer;
	strBuffer.assign(reinterpret_cast<const char*>(pBuffer), length);

	if (IsNULL(m_pClient))
	{
		return false;
	}
    dwSeqID = 0 ;
	bool bResult = true;
	switch(dwMsgID)
	{
        case MSGID_HOLD | ID_REQ:
            bResult = OnHold(dwSeqID,pBuffer,nLen);
            break;
        case MESSAGE_TYPE_CREATE_TABLE | MESSAGE_TYPE_REQ:
            bResult = OnREQCreateTable(dwSeqID,strBuffer,nLen);
            break;
        case MESSAGE_TYPE_JOIN_TABLE | MESSAGE_TYPE_REQ:
            bResult = OnREQJoinTable(dwSeqID,strBuffer,nLen);
            break;
        case MESSAGE_TYPE_READY_GAME | MESSAGE_TYPE_REQ:
            bResult = OnREQReadyGame(dwSeqID,strBuffer,nLen);
            break;
        case MESSAGE_TYPE_START_GAME | MESSAGE_TYPE_REQ:
            bResult = OnREQStartGame(dwSeqID,strBuffer,nLen);
            break;
        case MESSAGE_TYPE_START_MATCH | MESSAGE_TYPE_REQ:
            bResult = OnREQMatchGame(dwSeqID, strBuffer, nLen);
            break;
        case MESSAGE_TYPE_CHANGE_NAME | MESSAGE_TYPE_REQ:
            bResult = OnREQChangeName(dwSeqID, strBuffer, nLen);
            break;
        case MESSAGE_TYPE_DRAW_CHESS | MESSAGE_TYPE_REQ:
            bResult= OnDrawChess(dwSeqID,strBuffer,nLen);
			break;
        case MESSAGE_TYPE_CANCEL_MATCH:
            bResult = OnNotifyCancelMatch(dwSeqID,strBuffer,nLen);
			break;
		case MESSAGE_TYPE_REGRET_CHESS | MESSAGE_TYPE_REQ:
			bResult=OnRegretChess(dwSeqID,strBuffer,nLen);
			break;
		case MESSAGE_TYPE_AGREE_REGRET_CHESS | MESSAGE_TYPE_REQ:
			bResult=OnREQAgreeRegretChess(dwSeqID,strBuffer,nLen);
			break;
		case MESSAGE_TYPE_DISAGREE_REGRET_CHESS | MESSAGE_TYPE_REQ:
			bResult=OnREQDisagreeRegretChess(dwSeqID,strBuffer,nLen);
			break;
		case MESSAGE_TYPE_NOTIFY_GIVE_UP:
		    bResult=OnNotifyGiveUp(dwSeqID,strBuffer,nLen);
			break;
		case MESSAGE_TYPE_NOTIFY_PEACE_CHESS:
			bResult=OnNotifyPeaceChess(dwSeqID,strBuffer,nLen);
			break;
		case MESSAGE_TYPE_NOTIFY_AGREE_PEACE_CHESS:
			bResult=OnNotifyAgreePeaceChess(dwSeqID,strBuffer,nLen);
			break;
		case MESSAGE_TYPE_NOTIFY_DISAGREE_PEACE_CHESS:
			bResult=OnNotifyDisagreePeaceChess(dwSeqID,strBuffer,nLen);
			break;
		case MESSAGE_TYPE_REQ_LEADER_BOARD_INFO | MESSAGE_TYPE_REQ:
			bResult=OnREQLeaderBoardInfo(dwSeqID,strBuffer,nLen);
			break;
        default:
            break;
    }
	
	return bResult;
}

bool CPlayer::OnREQLeaderBoardInfo(DWORD dwSeqID, std::string& pBuffer, int nLen) 
{
	REQLeaderboard req;
    req.ParseFromString(pBuffer);

	LOG("CPlayer", "OnREQCreateTable", "玩家:%d请求获取排行榜信息", m_nUserID);
	m_pService->SendSvrdMsg(m_pService->GetAuthSvrdMsgRoute(), MESSAGE_TYPE_REQ_LEADER_BOARD_INFO | MESSAGE_TYPE_REQ, 0, m_nUserID, pBuffer, m_pService->GetClientID());
	return true;
}

bool CPlayer::OnREQChangeName(DWORD dwSeqID, std::string& pBuffer, int nLen)
{
	REQChangeName req;
    req.ParseFromString(pBuffer);

	LOG("CPlayer", "OnREQCreateTable", "玩家:%d请求修改昵称", m_nUserID);
	m_pService->SendSvrdMsg(m_pService->GetAuthSvrdMsgRoute(), MESSAGE_TYPE_CHANGE_NAME | MESSAGE_TYPE_REQ, 0, m_nUserID, pBuffer, m_pService->GetClientID());
	return true;
}

bool CPlayer::OnHold(DWORD dwSeqID, const byte* pBuffer, int nLen)
{
	ACKHold ack;
	ack.set_result_id(0);
	SendMsg(MSGID_HOLD | ID_ACK,&ack,dwSeqID);
	return true;
}

bool CPlayer::OnREQCreateTable(DWORD dwSeqID, std::string& pBuffer, int nLen)
{
	REQCreateTable req;
    req.ParseFromString(pBuffer);
    
	LOG("CPlayer", "OnREQCreateTable", "玩家:%d请求创建房间", m_nUserID);
	m_pService->SendSvrdMsg(m_pService->GetGameSvrdMsgRoute(), MESSAGE_TYPE_CREATE_TABLE | MESSAGE_TYPE_REQ, 0, m_nUserID, pBuffer, m_pService->GetClientID());
	return true;
}

bool CPlayer::OnREQJoinTable(DWORD dwSeqID, std::string& pBuffer, int nLen)
{
	LOG("CPlayer", "OnREQJoinTable", "玩家:%d请求加入房间", m_nUserID);
	m_pService->SendSvrdMsg(m_pService->GetGameSvrdMsgRoute(), MESSAGE_TYPE_JOIN_TABLE | MESSAGE_TYPE_REQ, 0, m_nUserID, pBuffer, m_pService->GetClientID());
	return true;
}

bool CPlayer::OnREQReadyGame(DWORD dwSeqID, std::string& pBuffer, int nLen)
{
	LOG("CPlayer", "OnREQReadyGame", "玩家:%d请求准备", m_nUserID);
	m_pService->SendSvrdMsg(m_pService->GetGameSvrdMsgRoute(), MESSAGE_TYPE_READY_GAME | MESSAGE_TYPE_REQ, 0, m_nUserID, pBuffer, m_pService->GetClientID());
	return true;
}

bool CPlayer::OnREQStartGame(DWORD dwSeqID, std::string& pBuffer, int nLen)
{
	LOG("CPlayer", "OnREQStartGame", "玩家:%d请求开始", m_nUserID);
	m_pService->SendSvrdMsg(m_pService->GetGameSvrdMsgRoute(), MESSAGE_TYPE_START_GAME | MESSAGE_TYPE_REQ, 0, m_nUserID, pBuffer, m_pService->GetClientID());
	return true;
}

bool CPlayer::OnDrawChess(DWORD dwSeqID, std::string& pBuffer, int nLen){
	LOG("CPlayer", "OnDrawChess", "玩家:%d请求下棋", m_nUserID);
	m_pService->SendSvrdMsg(m_pService->GetGameSvrdMsgRoute(), MESSAGE_TYPE_DRAW_CHESS | MESSAGE_TYPE_REQ, 0, m_nUserID, pBuffer, m_pService->GetClientID());
	return true;
}

bool CPlayer::OnREQMatchGame(DWORD dwSeqID, std::string& pBuffer, int nLen)
{
    LOG("CPlayer", "OnREQMatchGame", "玩家:%d请求匹配", m_nUserID);
    m_pService->SendSvrdMsg(m_pService->GetRoomSvrdMsgRoute(), MESSAGE_TYPE_START_MATCH | MESSAGE_TYPE_REQ, 0, m_nUserID, pBuffer, m_pService->GetClientID());
    return true;
}

bool CPlayer::OnNotifyCancelMatch(DWORD dwSeqID, std::string& pBuffer, int nLen)
{
    LOG("CPlayer", "OnNotifyCancelMatch", "玩家:%d通知取消匹配", m_nUserID);
    m_pService->SendSvrdMsg(m_pService->GetRoomSvrdMsgRoute(), MESSAGE_TYPE_CANCEL_MATCH, 0, m_nUserID, pBuffer, m_pService->GetClientID());
    return true;
}

bool CPlayer::OnRegretChess(DWORD dwSeqID, std::string& pBuffer, int nLen)
{
	LOG("CPlayer", "OnRegretChess", "玩家:%d悔棋", m_nUserID);
	m_pService->SendSvrdMsg(m_pService->GetGameSvrdMsgRoute(), MESSAGE_TYPE_REGRET_CHESS | MESSAGE_TYPE_REQ, 0, m_nUserID, pBuffer, m_pService->GetClientID());
    return true;
}
bool CPlayer::OnREQAgreeRegretChess(DWORD dwSeqID, std::string& pBuffer, int nLen)
{
	LOG("CPlayer", "OnREQAgreeRegretChess", "玩家:%d同意悔棋", m_nUserID);
	m_pService->SendSvrdMsg(m_pService->GetGameSvrdMsgRoute(), MESSAGE_TYPE_AGREE_REGRET_CHESS | MESSAGE_TYPE_REQ, 0, m_nUserID, pBuffer, m_pService->GetClientID());
    return true;
}
bool CPlayer::OnREQDisagreeRegretChess(DWORD dwSeqID, std::string& pBuffer, int nLen)
{
	LOG("CPlayer", "OnREQDisagreeRegretChess", "玩家:%d不同意悔棋", m_nUserID);
	m_pService->SendSvrdMsg(m_pService->GetGameSvrdMsgRoute(), MESSAGE_TYPE_DISAGREE_REGRET_CHESS | MESSAGE_TYPE_REQ, 0, m_nUserID, pBuffer, m_pService->GetClientID());
    return true;
}

bool CPlayer::OnNotifyGiveUp(DWORD dwSeqID, std::string& pBuffer, int nLen)
{
	LOG("CPlayer", "OnNotifyGiveUp", "玩家:%d认输", m_nUserID);
	m_pService->SendSvrdMsg(m_pService->GetGameSvrdMsgRoute(), MESSAGE_TYPE_NOTIFY_GIVE_UP, 0, m_nUserID, pBuffer, m_pService->GetClientID());
    return true;
}

bool CPlayer::OnNotifyPeaceChess(DWORD dwSeqID, std::string& pBuffer, int nLen)
{
	LOG("CPlayer", "OnNotifyPeaceChess", "玩家:%d求和", m_nUserID);
	m_pService->SendSvrdMsg(m_pService->GetGameSvrdMsgRoute(), MESSAGE_TYPE_NOTIFY_PEACE_CHESS, 0, m_nUserID, pBuffer, m_pService->GetClientID());
    return true;
}

bool CPlayer::OnNotifyAgreePeaceChess(DWORD dwSeqID, std::string& pBuffer, int nLen){
	LOG("CPlayer", "OnNotifyAgreePeaceChess", "玩家:%d同意求和", m_nUserID);
	m_pService->SendSvrdMsg(m_pService->GetGameSvrdMsgRoute(), MESSAGE_TYPE_NOTIFY_AGREE_PEACE_CHESS, 0, m_nUserID, pBuffer, m_pService->GetClientID());
    return true;
}

bool CPlayer::OnNotifyDisagreePeaceChess(DWORD dwSeqID, std::string& pBuffer, int nLen){
	LOG("CPlayer", "OnNotifyDisagreePeaceChess", "玩家:%d不同意求和", m_nUserID);
	m_pService->SendSvrdMsg(m_pService->GetGameSvrdMsgRoute(), MESSAGE_TYPE_NOTIFY_DISAGREE_PEACE_CHESS, 0, m_nUserID, pBuffer, m_pService->GetClientID());
    return true;
}