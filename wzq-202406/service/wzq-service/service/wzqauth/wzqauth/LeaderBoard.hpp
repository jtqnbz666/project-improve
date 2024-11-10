#ifndef LEADERBOARD_H
#define LEADERBOARD_H
#include "AppDef.h"
#include "AuthService.hpp"
#include "log.h"
#include <iostream>
#include "util.hpp"
class CLeaderboard {
public:
    static bool UpdateBoard(int64 originScore, int64 uid, const std::string &BoardKey);
    static ACKLeaderboard GetBoard(const REQLeaderboard &msg, const std::string &boardKey);
    static bool UpdateScoreInBoard(const std::string& uid, double score, const std::string& key);

    static double CalcScore(int64 originScore, int64_t timestamp);
};
#endif