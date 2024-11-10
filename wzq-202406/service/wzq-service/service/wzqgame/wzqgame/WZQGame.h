#ifndef __WZQ_GAME__
#define __WZQ_GAME__

#include <iostream>
#include "RedisIntf.h"
#include "ILog.h"
#include "mcgevent.h"
#include <string>
#include "AppDef.h"
#include "RedisIntf.h"
#include "log.h"
#include "mcgtimer.h"
#include "TableMgr.h"

// 步时30s
#define STEP_TIME 30

class CTable;
class CPlayer;
class CWZQGame
{
public :
    CWZQGame(ILog* pLog){{m_pLog = pLog;}}
    CWZQGame();
    ~CWZQGame();
    //初始化
    virtual int OnInit(bool bInitModel);
    virtual int OnTimer() ;
    virtual void OnGameStart();
    void OnUpdateTableStatus();
    void GenStatusInfo(UpdateTableStatus& tableStatus);
    void GenBaseInfo(UserRawData* playerBaseInfo, CPlayer* player); 
    virtual int OnClientMessage(int64 user_id, DWORD dwMsgID, DWORD dwSeqID, const byte* pBuffer, int nLen);
    bool AllPlayersReady();
    static LPTHREAD_START_ROUTINE PStepTimer(LPVOID arg){
        CWZQGame* game = (CWZQGame*)arg;
        if (--game->m_iStepTimer == 0) {
            game->OnPStepTimerOut();
        }
        return 0;
    }
    DWORD OnPStepTimerOut();
    void ChangePlayer();
    void OnGameEnd();
public:
    CPlayer* m_pPlayer1;
    CPlayer* m_pPlayer2;
    CPlayer* m_pCurrentPlayer;
public:
    ILog * m_pLog;
    CTable* m_pTable ;
    HANDLE m_hStepTimer;
    int m_iStepTimer = STEP_TIME;
};

#endif
