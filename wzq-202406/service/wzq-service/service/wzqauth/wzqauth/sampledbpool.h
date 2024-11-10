#ifndef __SAMPLE_DB_POOL_H__
#define __SAMPLE_DB_POOL_H__

#include <mysql/mysql.h>
#include <string>
#include <google/protobuf/stubs/common.h>
#include "AppDef.h"
#include "LeaderBoard.hpp"
#include <iostream>
#include <string>
#include <sstream>
#include "AuthService.hpp"

#define IN
#define OUT
#define INOUT
#define MAX_SQL_LEN	4*1024
#define SQL_SUCCESS 0
#define SQL_FAILED  -1

class CSampleDBPool
{
public:
	CSampleDBPool();
	~CSampleDBPool();

public:
	//
    int Connect(const string &strHost, 
            const string &strUser, 
            const string &strPwd, 
            const string &strDBName, 
            unsigned int nPort, 
            const string &strCharSet="utf8");

    int Close();

    int Ping();

    int Commit();

    int Rollback();
    
    // 设备信息存储
    int UpdateDeviceInfo(const std::string& deviceid , const std::string& info);
    // 查询用户账号信息
    int QueryUserInfo(const std::string& account , int64& userid, std::string& passwd,std::string& salt_hex, std::string& displayID);
    // 根据ID查询用户账号信息
    int QueryUserInfoByID(int64& userid, std::string& passwd, std::string& phoneNumber, std::string& displayID);
    // 根据手机号查询用户账号信息
    int QueryUserInfoByPhoneNumber(int64& userID, const std::string& phoneNumber);
    // 插入用户账号信息
    int InsertUserInfo(const std::string& account , const std::string& passwd, const std::string& displayID, const std::string& phoneNumber);
    // 更改玩家昵称
    int UpdateUserName(int64& userID, const std::string& newName);
    // 组装前端需要的信息
    int QueryUserRawDataInfo(int64& userID, UserRawData &baseData);
    //游戏结算
    int UpdatePlayingResult(int64& gameResult,int64& winUserID,int64& loseUserID,int64& color,std::vector<std::pair<int, int>> movesData);

private:
    CSampleDBPool(const CSampleDBPool &dbpool);
    CSampleDBPool & operator=(const CSampleDBPool &dbpool); 

private:
    MYSQL *m_hMySQL;
	char m_szSQL[MAX_SQL_LEN];
	bool m_bPrint;
};

#endif //__SAMPLE_DB_POOL_H__


