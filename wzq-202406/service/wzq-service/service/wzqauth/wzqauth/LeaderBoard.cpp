#include "LeaderBoard.hpp"

extern CAuthService *g_pService; // 声明外部变量
const double BaseTime = 6666666666666.0;
using namespace std::chrono;
bool CLeaderboard::UpdateBoard(int64 originScore, int64 uid, const std::string &BoardKey) {
    std::string uidString = std::to_string(uid);
    milliseconds ms = duration_cast<milliseconds>(system_clock::now().time_since_epoch());
    double compareScore = CalcScore(originScore, ms.count());
    LOG("CLeaderboard", "UpdateBoard", "尝试上榜, uid:%d, originScore:%d, compareSocre:%.13f",
    uid, originScore, compareScore);
    // todo:检查是否合规
    UpdateScoreInBoard(uidString, compareScore, BoardKey);
    return false;   
}

// 更新排行榜分数的函数
bool CLeaderboard::UpdateScoreInBoard(const std::string& uid, double score, const std::string& key) {
    redisReply* reply = static_cast<redisReply*>(redisCommand(g_pService->GetRedisIns(), 
        "ZADD %s %.13f %s", key.c_str(), score, uid.c_str()));

    if (!reply) {
        std::cerr << "Redis command failed: " << g_pService->GetRedisIns()->errstr << std::endl;
        return false;
    }

    bool success = (reply->type != REDIS_REPLY_ERROR);
    freeReplyObject(reply);
    return success;
}

ACKLeaderboard CLeaderboard::GetBoard(const REQLeaderboard &msg, const std::string &boardKey) {
    ACKLeaderboard result;
    result.set_resultcode(RESULT_CODE_SUCCESS); // 默认成功

    if (g_pService->GetRedisIns() == nullptr) {
        std::cerr << "Redis context is not initialized." << std::endl;
        result.set_resultcode(-1); // Redis上下文未初始化
        return result;
    }
    
    // 使用 ZREVRANGE 命令来获取指定范围的元素及其分数
    redisReply* reply = (redisReply*)redisCommand(g_pService->GetRedisIns(), 
        "ZREVRANGE %s %d %d WITHSCORES", 
        boardKey.c_str(), 
        0,
        9
    );

    if (reply == nullptr) {
        std::cerr << "Redis command failed: " << g_pService->GetRedisIns()->errstr << std::endl;
        result.set_resultcode(-3); // Redis 命令失败
        return result;
    }

    if (reply->type == REDIS_REPLY_ARRAY) {
        // 遍历回复的数组，提取键值对及其分数
        for (size_t i = 0; i < reply->elements; i += 2) {
            LeaderboardEntity* entity = result.add_lbentities(); // 添加新实体到结果中
            entity->set_rank(static_cast<int32_t>(i / 2 + 1)); // 排名从1开始
            entity->set_score(std::stod(reply->element[i + 1]->str));
            // 获取玩家信息
            auto baseInfo = entity->mutable_personinfo();
            *baseInfo = g_pService->GetBaseInfo(std::stoull(reply->element[i]->str));
        }
        result.set_resultcode(RESULT_CODE_SUCCESS); // 成功
    } else {
        std::cerr << "Unexpected reply type: " << reply->type << std::endl;
        result.set_resultcode(-4); // 回复类型错误
    }

    freeReplyObject(reply);
    return result;
}

double CLeaderboard::CalcScore(int64 originScore, int64_t timestamp) {
    double finalScore = static_cast<double>(originScore) + (static_cast<double>(BaseTime) - static_cast<double>(timestamp)) * (1.0 / static_cast<double>(BaseTime));
    printf("转换后的分数: %.10f\n", finalScore);
    return finalScore;
}