

#include "WZQGame.h"
#include "GameService.h"
#include <json/json.h>
#include "Table.h"
#include "Player.h"

using namespace wzq;
extern CGameService* g_pService ;

CWZQGame::CWZQGame()
{
}

CWZQGame::~CWZQGame()
{
    if (IsNotNULL(m_hStepTimer)) {
        CMcgTimer::DeleteTimer(m_hStepTimer, true);
    }
}

int CWZQGame::OnInit(bool bInitModel)
{
    return true;
}

//定时器
int CWZQGame::OnTimer()
{
    return 0;
}

int CWZQGame::OnClientMessage(int64 user_id, DWORD dwMsgID, DWORD dwSeqID, const byte* pBuffer, int nLen)
{
    bool bHandle = false;
    
    switch (dwMsgID) {
        default:
            break;
    }
    return bHandle ? 0 : -1;
}

//bool CWZQGame::AllPlayersReady() {
//    return m_pPlayer1->m_bIsReady && m_pPlayer2->m_bIsReady;
//}


//void CWZQGame::OnGameStart() {
//    NotifyStartGame msg;
//    // 需要提供给玩家的信息
//    msg.set_color(m_pPlayer1->m_cColor);
//    m_pTable->m_pService->SendSvrdMsg(m_pTable->m_pService->GetConnSvrdMsgRoute(), 
//    static_cast<int64>(MESSAGE_TYPE_NOTIFY_START_GAME), 0, m_pPlayer1->m_nUserID, msg.SerializeAsString(), 0);
//    
//    msg.set_color(m_pPlayer2->m_cColor);
//    m_pTable->m_pService->SendSvrdMsg(m_pTable->m_pService->GetConnSvrdMsgRoute(),
//    static_cast<int64>(MESSAGE_TYPE_NOTIFY_START_GAME), 0, m_pPlayer2->m_nUserID, msg.SerializeAsString(), 0);
//}

// void CWZQGame::OnGameStart() {
//     NotifyStartGame msg;
//     // 需要提供给玩家的信息
//     msg.set_color(m_pPlayer1->m_cColor);
//     m_pTable->m_pService->SendSvrdMsg(m_pTable->m_pService->GetConnSvrdMsgRoute(), 
//     static_cast<int64>(MESSAGE_TYPE_NOTIFY_START_GAME), 0, m_pPlayer1->m_nUserID, msg.SerializeAsString(), 0);
    
//     msg.set_color(m_pPlayer2->m_cColor);
//     m_pTable->m_pService->SendSvrdMsg(m_pTable->m_pService->GetConnSvrdMsgRoute(),
//     static_cast<int64>(MESSAGE_TYPE_NOTIFY_START_GAME), 0, m_pPlayer2->m_nUserID, msg.SerializeAsString(), 0);
// }


void CWZQGame::GenBaseInfo(UserRawData* playerBaseInfo, CPlayer* player) {
    LOG("CWZQGame", "GenBaseInfo", "userId = %llu", player->m_nUserID);
    *playerBaseInfo = m_pTable->m_pService->GetBaseInfo(player->m_nUserID);
}

// 组装玩家状态信息
void CWZQGame::GenStatusInfo(UpdateTableStatus& tableStatus) {
    PlayerStatus* playerAStatus = tableStatus.add_players(); 
    PlayerStatus* playerBStatus = tableStatus.add_players(); 
    
    UserRawData* playerABaseInfo = playerAStatus->mutable_baseinfo();  
    UserRawData* playerBBaseInfo = playerBStatus->mutable_baseinfo();

    GenBaseInfo(playerABaseInfo, m_pPlayer1);
    playerAStatus->set_state(m_pPlayer1->m_eGameState);

    GenBaseInfo(playerBBaseInfo, m_pPlayer2);
    playerBStatus->set_state(m_pPlayer2->m_eGameState);

    GenBaseInfo(playerABaseInfo, m_pPlayer1);
    playerAStatus->set_color(m_pPlayer1->m_cColor);

    GenBaseInfo(playerBBaseInfo, m_pPlayer2);
    playerBStatus->set_color(m_pPlayer2->m_cColor);

}

void CWZQGame::OnUpdateTableStatus() {
    UpdateTableStatus tableStatus;   
    GenStatusInfo(tableStatus);

    m_pTable->m_pService->SendSvrdMsg(m_pTable->m_pService->GetConnSvrdMsgRoute(), 
    static_cast<int64>(MESSAGE_TYPE_UPDATE_TABLE_STATUS), 0, m_pPlayer1->m_nUserID, tableStatus.SerializeAsString(), 0);
    
    m_pTable->m_pService->SendSvrdMsg(m_pTable->m_pService->GetConnSvrdMsgRoute(),
     static_cast<int64>(MESSAGE_TYPE_UPDATE_TABLE_STATUS), 0, m_pPlayer2->m_nUserID, tableStatus.SerializeAsString(), 0);
}

DWORD CWZQGame::OnPStepTimerOut()
{
    LOG("CWZQGame", "OnPStepTimerOut", "enter");
    
    // 胜负通知 todo 转发auth服
    m_pCurrentPlayer->m_eGameState = GameState::GS_LOSE;
    m_pPlayer1 == m_pCurrentPlayer ? m_pPlayer2->m_eGameState = GameState::GS_WIN : m_pPlayer1->m_eGameState = GameState::GS_WIN;

    auto table = m_pTable->m_pService->m_pTableMgr->GetTable(m_pCurrentPlayer->m_sTableID);

    if(table->m_bOnline){
	PlayingResult result;
	result.set_gameresult(m_pCurrentPlayer->m_eGameState);
	
    m_pPlayer1 == m_pCurrentPlayer ? result.set_winuserid(m_pPlayer2->m_nUserID) : result.set_winuserid(m_pPlayer1->m_nUserID);
	result.set_color(m_pPlayer1->m_cColor);

// 遍历 m_vChessPositions 中的每个棋子位置信息
	for (const auto& position : table->m_vChessPositions) {
		ChessMove* move = result.add_moves();
		move->set_x(position[0].first); // 存储 x 值
		move->set_y(position[0].second); // 存储 y 值
	}
	result.set_loseuserid(m_pCurrentPlayer->m_nUserID);
	
	table->m_pGame->m_pTable->m_pService->SendSvrdMsg(table->m_pGame->m_pTable->m_pService->GetAuthSvrdMsgRoute(),
    	static_cast<int64>(MESSAGE_TYPE_NOTIFY_GAME_SETTLEMENT), 0, m_pCurrentPlayer->m_nUserID, result.SerializeAsString(), 0);
	}
	
	for (int i = 0; i < 15; ++i) {
        for (int j = 0; j < 15; ++j) {
				table->m_iChessMatrix[i][j] = 0;}
	}
	table->m_vChessPositions.clear();
    
    OnUpdateTableStatus();
    
    OnGameEnd();
}

void CWZQGame::ChangePlayer()
{
    LOG("CWZQGame", "ChangePlayer", "enter");
    
    // 更换下棋手
    m_pCurrentPlayer == m_pPlayer1 ? m_pCurrentPlayer = m_pPlayer2 : m_pCurrentPlayer = m_pPlayer1;
    // 重置定时器
    m_iStepTimer = STEP_TIME;
}

void CWZQGame::OnGameEnd()
{
    LOG("CWZQGame", "StopTimer", "enter");
    
    if (IsNotNULL(m_hStepTimer)) {
        CMcgTimer::DeleteTimer(m_hStepTimer, true);
    }
}

void CWZQGame::OnGameStart()
{
    LOG("CWZQGame", "OnGameStart", "enter");
    
    // 启动步时定时器
    m_iStepTimer = STEP_TIME;
    m_pCurrentPlayer = (m_pPlayer1->m_cColor == ChessColorType::Chess_Black ? m_pPlayer1 : m_pPlayer2);
    m_hStepTimer =  CMcgTimer::CreateTimer((LPTHREAD_START_ROUTINE)PStepTimer, this, 1000,  1000);
}
