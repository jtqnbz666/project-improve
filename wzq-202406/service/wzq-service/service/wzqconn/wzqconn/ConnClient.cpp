#include "ConnClient.h"
#include "ConnService.h"
#include "log.h"
#include <iconv.h>
#include <errno.h>
#include <arpa/inet.h>

#include "Tools.h"

extern CService *g_pService;

CConnClient::CConnClient(DWORD dwRemoteIP,WORD wListenPort):CMCGClient(dwRemoteIP,wListenPort)
{
    m_pService = (CConnService*)g_pService;
	m_pPlayer = NULL;
    m_bLogined = false ;
}

CConnClient::~CConnClient()
{
}

bool CConnClient::OnInit()
{
	m_pService=(CConnService *)g_pService;
	if (!CMCGClient::OnInit()) return false;
	return true;
}

void CConnClient::OnForwardMsgToAuthServer(DWORD dwMsgID, DWORD dwSeqID, string strBuffer) {
    LOG("CConnClient", "OnForwardMsgToAuthServer", "转发消息 %d 到校验服", dwMsgID);
    m_pService ->SendSvrdMsg(m_pService->GetAuthSvrdMsgRoute(), dwMsgID, 0, 0, strBuffer, GetClientID());
}

bool CConnClient::OnMsg(DWORD dwMsgID, DWORD dwSeqID)
{
    auto& client = StatsdClientSingleton::getInstance();
    client.increment("ConnServer.req.all.count");
    LOG("CPlayer", "OnMsg", "收到消息:%s", to_string(dwMsgID).c_str());
	if (CMCGClient::OnMsg(dwMsgID, dwSeqID)) return true;
    dwSeqID = 0 ;
    string strBuffer;
	McgPopPacket(strBuffer);
    
    switch(dwMsgID) {
    case (MSGID_REGISTER | ID_REQ):
    case (MSGID_PWD_LOGIN | ID_REQ):
    case (MSGID_TOKEN_LOGIN | ID_REQ):
    case (MESSAGE_TYPE_PHONE_NUMBER_LOGIN | MESSAGE_TYPE_REQ):
    case (MESSAGE_TYPE_SEND_VERIFY_CODE | MESSAGE_TYPE_REQ):
        OnForwardMsgToAuthServer(dwMsgID, dwSeqID, strBuffer);
        break;
    default:
        m_pPlayer->OnMsg(dwMsgID, dwSeqID, (const byte*)strBuffer.data(), (int)strBuffer.length());
        break;
    }

    std::string statKey = StatsUtils::GetStatKeyFromMsgID(dwMsgID);
    client.increment(statKey);
    
    return true;
}

bool CConnClient::OnBreak(DWORD dwType)
{
    if(m_pPlayer)
    {
        m_pPlayer ->Disconnect();
        m_pPlayer = nullptr;
    }
    return true ;
}

bool CConnClient::OnTimeOut(DWORD dwEventID)
{
	if (CMCGClient::OnTimeOut(dwEventID)) 
	{
		return true;
	}
	return true;
}

