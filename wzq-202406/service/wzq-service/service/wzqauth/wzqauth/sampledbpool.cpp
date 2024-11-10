#include "sampledbpool.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "log.h"
#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <openssl/sha.h>
#include <openssl/rand.h>
#include <ctime>
extern CAuthService *g_pService; // 声明外部变量
CSampleDBPool::CSampleDBPool()
: m_hMySQL(NULL), m_bPrint(true)
{
	memset(m_szSQL, 0, MAX_SQL_LEN);

	//mysql_library_int();

	m_hMySQL = mysql_init(0);
	if (NULL == m_hMySQL)
	{
		printf("mysql_init err(%s)\n", mysql_error(m_hMySQL));
	}
}

CSampleDBPool::~CSampleDBPool()
{
	Close();
}

int CSampleDBPool::Connect(const string &strHost, 
						   const string &strUser, 
						   const string &strPwd, 
						   const string &strDBName, 
						   unsigned int nPort, 
						   const string &strCharSet)
{
	//
	if (m_hMySQL != mysql_real_connect(m_hMySQL, strHost.c_str(), strUser.c_str(), strPwd.c_str(), strDBName.c_str(), nPort, 0, 0))
	{
		printf("mysql_real_connect err(%s)\n", mysql_error(m_hMySQL));
		return SQL_FAILED;
	}

	//
	if (mysql_set_character_set(m_hMySQL, strCharSet.c_str()))
	{
		printf("mysql_set_character_set err(%s)\n", mysql_error(m_hMySQL));
		return SQL_FAILED;
	}

	char value = 1 ;
	int ret = mysql_options(m_hMySQL, MYSQL_OPT_RECONNECT, (char *)&value);
	printf("mysql_options ret : %d", ret);

	return SQL_SUCCESS;
}

int CSampleDBPool::Close()
{
	if (m_hMySQL)
	{
		mysql_close(m_hMySQL);
		m_hMySQL = NULL;
	}

	mysql_library_end();

	return SQL_SUCCESS;
}

int CSampleDBPool::Ping()
{
	if (mysql_ping(m_hMySQL))
	{
		printf("mysql_ping err(%s)\n", mysql_error(m_hMySQL));
		return SQL_FAILED;//
	}

	return SQL_SUCCESS;//
}

int CSampleDBPool::Commit()
{
    return mysql_commit(m_hMySQL);
}

int CSampleDBPool::Rollback()
{
    return mysql_rollback(m_hMySQL);
}

//设备信息存储
int CSampleDBPool::UpdateDeviceInfo(const std::string& deviceid , const std::string& info)
{
    mcgWriteLog("CSampleDBPool::UpdateDeviceInfo deviceid : %s , info : %s" , deviceid.c_str() , info.c_str());
    memset(m_szSQL, 0, MAX_SQL_LEN);
    int len = sprintf(m_szSQL, "insert into deviceinfo values('%s', '%s') on duplicate key update info = '%s'", deviceid.c_str(), info.c_str(), info.c_str());
    mcgWriteLog("CSampleDBPool::UpdateDeviceInfo sql : %s" , m_szSQL);
    int ret = mysql_real_query(m_hMySQL, m_szSQL, len);
    if (IsNotZero(ret))
    {
        mcgWriteLog("Error exec query: %s \n", mysql_error(m_hMySQL));
        return SQL_FAILED;
    }
    else
    {
        mcgWriteLog("exec query successfully");
        return SQL_SUCCESS;
    }
    return 0;
}
static string sha256_salt(const std::string& passwd, const std::string& salt){
    std::string salted_password = passwd + salt;
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, salted_password.c_str(), salted_password.length());
    SHA256_Final(hash, &sha256);

    std::stringstream ss;
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        ss << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
    }

    return ss.str();
}
//查询用户账号信息
int CSampleDBPool::QueryUserInfo(const std::string& account, int64& userID,
 std::string& password,std::string& saltHex, std::string& displayID)
{
    memset(m_szSQL, 0, MAX_SQL_LEN);
    // 使用 mysql_real_escape_string 防止 SQL 注入
    char escaped_account[2 * account.length() + 1];
    mysql_real_escape_string(m_hMySQL, escaped_account, account.c_str(), account.length());

    // 构建查询语句
    int len = snprintf(m_szSQL, MAX_SQL_LEN,
                       "SELECT ID, passwd, salt_hex, display_id FROM user WHERE account = '%s'", escaped_account);

    int ret = mysql_real_query(m_hMySQL, m_szSQL, len);
    if (IsNotZero(ret))
    {
        mcgWriteLog("Error exec query: %s \n", mysql_error(m_hMySQL));
        return SQL_FAILED;
    }

    MYSQL_RES *results = mysql_store_result(m_hMySQL);
    if (!results)
    {
        mcgWriteLog("Error storing result: %s \n", mysql_error(m_hMySQL));
        return SQL_FAILED;
    }

    MYSQL_ROW record;
    while ((record = mysql_fetch_row(results)))
    {
        userID = atol(record[0]);
        password = record[1] ? record[1] : "";
        saltHex = record[2] ? record[2] : "";
        displayID = record[3] ? record[3] : "";
        break;
    }

    mysql_free_result(results);
    return 0;
}

// 根据uid获取用户信息
int CSampleDBPool::QueryUserInfoByID(int64& userid, std::string& passwd,std::string& phoneNumber, std::string& displayID)
{
    memset(m_szSQL, 0, MAX_SQL_LEN);
    int len = sprintf(m_szSQL, "select passwd, phone_number, display_id from user where ID = '%d'", userid);

    int ret = mysql_real_query(m_hMySQL, m_szSQL, len);
    if (IsNotZero(ret))
    {
        mcgWriteLog("Error exec query: %s \n", mysql_error(m_hMySQL));
        return SQL_FAILED;
    }
    MYSQL_RES *results = mysql_store_result(m_hMySQL);
    MYSQL_ROW record;
    while((record = mysql_fetch_row(results)))
    {
        passwd = record[0] ? record[0] : "";
        phoneNumber = record[1] ? record[1] : "";
        displayID = record[2] ? record[2] : "";
        break;
    }
    mysql_free_result(results);
    return 0;
}

// 根据手机号获取用户信息
int CSampleDBPool::QueryUserInfoByPhoneNumber(int64& userID, const std::string& phoneNumber)
{
    char m_szSQL[256];  
    int len = std::snprintf(m_szSQL, sizeof(m_szSQL), "select ID from user where phone_number = '%s'", phoneNumber.c_str());
    int ret = mysql_real_query(m_hMySQL, m_szSQL, len);
    if (IsNotZero(ret))
    {
        mcgWriteLog("Error exec query: %s \n", mysql_error(m_hMySQL));
        return SQL_FAILED;
    }
    
    MYSQL_RES *results = mysql_store_result(m_hMySQL);
    MYSQL_ROW record;
    while((record = mysql_fetch_row(results)))
    {
        userID = atol(record[0]);
        break;
    }

    mysql_free_result(results);
    return 0;
}


/*
 CREATE TABLE `user` (
  `ID` bigint(20) NOT NULL AUTO_INCREMENT,
  `account` varchar(64) NOT NULL,
  `passwd`  varchar(256) NOT NULL,
  PRIMARY KEY (`account`),
  KEY `ID` (`ID`)
) ENGINE=InnoDB AUTO_INCREMENT=20000 DEFAULT CHARSET=utf8
 */
//插入用户账号信息
int CSampleDBPool::InsertUserInfo(const std::string& account , const std::string& passwd, const std::string& displayID, const std::string& phoneNumber)
{
    unsigned char salt[16];
    RAND_bytes(salt, 16);
    stringstream ss;
    for (int i = 0; i < 16; i++) {
        ss << hex << setw(2) << setfill('0') << (int)salt[i];
    }
    string salt_hex = ss.str();
    string hashed_password = sha256_salt(passwd, salt_hex);
    mcgWriteLog("加密完成");
    memset(m_szSQL, 0, MAX_SQL_LEN);
    time_t now = time(0);
    tm *ltm = localtime(&now);
    std::string regist_time = std::to_string(1900 + ltm->tm_year) + "-" + std::to_string(1 + ltm->tm_mon) + "-" + std::to_string(ltm->tm_mday) + " " + std::to_string(ltm->tm_hour) + ":" + std::to_string(ltm->tm_min) + ":" + std::to_string(ltm->tm_sec);

    // 构建SQL语句
    char m_szSQL[MAX_SQL_LEN];
    int len = 0;
    if (phoneNumber != "") {
        len = sprintf(m_szSQL, "insert into user(account, name, phone_number, salt_hex, regist_time, display_id) values (NULL, NULL '%s', '%s', '%s', '%s')",
        phoneNumber.c_str(), salt_hex.c_str(), regist_time.c_str(), displayID.c_str());
    } else {
        len = sprintf(m_szSQL, "insert into user(account, name, passwd, salt_hex, regist_time, display_id) values ('%s', NULL, '%s', '%s', '%s', '%s')",
        account.c_str(), hashed_password.c_str(), salt_hex.c_str(), regist_time.c_str(), displayID.c_str());
    }
    
    // 执行SQL查询
    if (mysql_real_query(m_hMySQL, m_szSQL, len) != 0) {
        mcgWriteLog("Error exec query: %s \n", mysql_error(m_hMySQL));
        return SQL_FAILED;
    }

    return 0;
}

// 修改昵称
int CSampleDBPool::UpdateUserName(int64& userID, const std::string& newName) {
    // 先查是否存在
    char query[256];
    
    // 检查是否存在相同的 name
    sprintf(query, "SELECT COUNT(*) FROM user WHERE name = '%s';", newName.c_str());
    if (mysql_query(m_hMySQL, query)) {
        fprintf(stderr, "SELECT error: %s\n", mysql_error(m_hMySQL));
        return -1; // 查询错误
    }
    
    MYSQL_RES* result = mysql_store_result(m_hMySQL);
    if (!result) {
        fprintf(stderr, "mysql_store_result error: %s\n", mysql_error(m_hMySQL));
        return -1; // 结果存储错误
    }
    
    MYSQL_ROW row = mysql_fetch_row(result);
    int count = atoi(row[0]);
    mysql_free_result(result);
    
    if (count > 0) {
        return RESULT_CODE_EXIST_SAME_NAME; // 已存在相同的 name
    }
    
    // 更新指定 userID 玩家 name
    sprintf(query, "UPDATE user SET name = '%s' WHERE ID = %lld;", newName.c_str(), userID);
    if (mysql_query(m_hMySQL, query)) {
        fprintf(stderr, "UPDATE error: %s\n", mysql_error(m_hMySQL));
        return -1; // 更新错误
    }
    
    return RESULT_CODE_SUCCESS; // 成功
}

int CSampleDBPool::QueryUserRawDataInfo(int64& userID, UserRawData &baseData) {
    memset(m_szSQL, 0, MAX_SQL_LEN);
    
    // 组装玩家基本信息
    int len = sprintf(m_szSQL, "select name, display_id, score, highest_score from user where ID = '%d'", userID);
    int ret = mysql_real_query(m_hMySQL, m_szSQL, len);
    if (IsNotZero(ret))
    {
        mcgWriteLog("Error exec query: %s \n", mysql_error(m_hMySQL));
        return SQL_FAILED;
    }
    MYSQL_RES *results = mysql_store_result(m_hMySQL);
    MYSQL_ROW record;
    while((record = mysql_fetch_row(results)))
    {
        baseData.set_name(record[0] ? record[0] : "");
        baseData.set_displayid(record[1] ? record[1] : "");
        baseData.set_score(atoi(record[2] ? record[2] : "0"));
        baseData.set_highestscore(atoi(record[3] ? record[3] : "0"));
        break;
    }

    mysql_free_result(results);

    // 查询总场次
    len = sprintf(m_szSQL, "select count(*) from user_game_record where user_id = '%lld'", userID);
    ret = mysql_real_query(m_hMySQL, m_szSQL, len);
    if (IsNotZero(ret)) {
        mcgWriteLog("Error exec query: %s \n", mysql_error(m_hMySQL));
        return SQL_FAILED;
    }
    results = mysql_store_result(m_hMySQL);
    int totalCount = 0;
    if ((record = mysql_fetch_row(results))) {
        totalCount = atoi(record[0] ? record[0] : "0");
    }
    mysql_free_result(results);

    // 查询胜利场次
    len = sprintf(m_szSQL, "select count(*) from user_game_record where user_id = '%lld' and is_winner = 1", userID);
    ret = mysql_real_query(m_hMySQL, m_szSQL, len);
    if (IsNotZero(ret)) {
        mcgWriteLog("Error exec query: %s \n", mysql_error(m_hMySQL));
        return SQL_FAILED;
    }
    results = mysql_store_result(m_hMySQL);
    int winCount = 0;
    if ((record = mysql_fetch_row(results))) {
        winCount = atoi(record[0] ? record[0] : "0");
    }
    mysql_free_result(results);

    // 设置游戏记录
    GameRecord* gameRecord = baseData.mutable_gamerecord();
    gameRecord->set_totalcount(totalCount);
    gameRecord->set_wincount(winCount);

    return 0;
}

int CSampleDBPool::UpdatePlayingResult(int64& gameResult,int64& winUserID,int64& loseUserID,int64& color,std::vector<std::pair<int, int>> movesData){
    LOG("CSampleDBPool", "UpdatePlayingResult", "更新数据库");
    LOG("CSampleDBPool", "UpdatePlayingResult", "游戏结果:%d,获胜id:%d,失败id:%d,Color:%d,movesData:%d",gameResult,winUserID,loseUserID,color,movesData[0].first);
    memset(m_szSQL, 0, MAX_SQL_LEN);
    std::stringstream ss;
    for (size_t i = 0; i < movesData.size(); ++i) {
        ss << "(" << movesData[i].first << "," << movesData[i].second << ")";
        if (i != movesData.size() - 1) {
            ss << ",";
        }
    }
    if(gameResult!=1){
        int len = sprintf(m_szSQL, "SELECT score, highest_score FROM user WHERE ID = %d", winUserID);
        int ret = mysql_real_query(m_hMySQL, m_szSQL, len);
        MYSQL_RES *results = mysql_store_result(m_hMySQL);
        MYSQL_ROW row = mysql_fetch_row(results);
        int score = atoi(row[0]);
        int highestScore = atoi(row[1]);
        // 根据 score 的值增加分数
        if (score >= 0 && score <= 100) {
            score += 3;
        } else if (score > 100 && score <= 200) {
            score += 5;
        } else if (score > 200) {
            score += 8;
        }
        memset(m_szSQL, 0, MAX_SQL_LEN);
        highestScore=max(highestScore,score);
        len = sprintf(m_szSQL, "UPDATE user SET score = %d, highest_score = %d WHERE ID = %d", score,highestScore,winUserID);
        // 更新胜利者排行榜分数
        CLeaderboard::UpdateBoard(score, winUserID, "RankingBoard");
        // 更新redis值
        auto baseInfo =  g_pService->GetBaseInfo(winUserID);
        baseInfo.set_score(score);
        baseInfo.set_highestscore(highestScore);
        g_pService->SetBaseInfo(baseInfo);
        

        ret = mysql_real_query(m_hMySQL, m_szSQL, len);
        mysql_free_result(results);
        memset(m_szSQL, 0, MAX_SQL_LEN);
        len = sprintf(m_szSQL, "SELECT score FROM user WHERE ID = %d", loseUserID);
        ret = mysql_real_query(m_hMySQL, m_szSQL, len);
        results = mysql_store_result(m_hMySQL);
        row = mysql_fetch_row(results);
        score = atoi(row[0]);
        // 根据 score 的值增加分数
        if (score >= 0 && score <= 100) {
            score -= 3;
        } else if (score > 100 && score <= 200) {
            score -= 5;
        } else if (score > 200) {
            score -= 8;
        }
        len = sprintf(m_szSQL, "UPDATE user SET score = %d WHERE ID = %d", score,loseUserID);
        ret = mysql_real_query(m_hMySQL, m_szSQL, len);
        mysql_free_result(results);

        // 更新失败者排行榜分数
        CLeaderboard::UpdateBoard(score, loseUserID, "RankingBoard");
        baseInfo = g_pService->GetBaseInfo(loseUserID);
        baseInfo.set_score(score);
        g_pService->SetBaseInfo(baseInfo);
    }
    memset(m_szSQL, 0, MAX_SQL_LEN);
    std::string history = ss.str();
    int len=sprintf(m_szSQL, "INSERT INTO game_details(history) VALUES ('%s')", history.c_str());
    int ret = mysql_real_query(m_hMySQL, m_szSQL, len);
    LOG("CSampleDBPool", "UpdatePlayingResult", "game_details插入结果:%d",ret);
    int lastInsertedID = mysql_insert_id(m_hMySQL);
    int is_winner = (gameResult == 1) ? -1 : 1;
    int is_black = (color == 2) ? 1 : 0;
    
    memset(m_szSQL, 0, MAX_SQL_LEN);
    LOG("CSampleDBPool", "UpdatePlayingResult", "game_id%d, user_id%lld, rival_id%lld, is_winner%d, is_black%d",lastInsertedID, winUserID, loseUserID, is_winner, is_black);
    len = sprintf(m_szSQL, "INSERT INTO user_game_record(game_id, user_id, rival_id, is_winner, is_black, end_time) VALUES (%d, %lld, %lld, %d, %d, NOW())",
        lastInsertedID, winUserID, loseUserID, is_winner, is_black);
    ret = mysql_real_query(m_hMySQL, m_szSQL, len);
    LOG("CSampleDBPool", "UpdatePlayingResult", "user_game_record插入结果:%d",ret);
    is_winner = (gameResult == 1) ? -1 : 0;
    is_black=(is_black==1)?0:1;
    memset(m_szSQL, 0, MAX_SQL_LEN);
    len = sprintf(m_szSQL, "INSERT INTO user_game_record(game_id, user_id, rival_id, is_winner, is_black, end_time) VALUES (%d, %lld, %lld, %d, %d, NOW())",
        lastInsertedID, loseUserID, winUserID, is_winner, is_black);
    ret = mysql_real_query(m_hMySQL, m_szSQL, len);
    LOG("CSampleDBPool", "UpdatePlayingResult", "user_game_record插入结果:%d",ret);
    return 0;
}












