#include "HeroConfig.h"

// 此文件由 tools/generate.py 自动生成，请勿手动修改。

std::unordered_map<int, SHeroConfig> CHeroConfig::_InitData() {
    std::unordered_map<int, SHeroConfig> mTempData;

    {
        SHeroConfig oConfig;
    
        oConfig.nId = 1;
    
        oConfig.sName = "英雄";
    
        oConfig.fValueFloat = 9.91;
    
        oConfig.vTestint = {1, 2};
    
        oConfig.vKeywords = {"newbie", "gift"};
    
        oConfig.mStrToIntMap = {{"gold", 100}, {"diamond", 10}};
    
        oConfig.vStrPairArray = {{"exp", 500}, {"stamina", 101}};
    
        oConfig.sComplexLogic = "{\"level\": 10, \"vip_only\": true, \"unlock_condition\": \"FINISH_QUEST_101\"}";
    
        oConfig.sTestjson = "{\"dealcardweightinfo\": {\"1\": {\"jokercardweight\": 1, \"keycardweight\": 0, \"mainsecondarycardweight\": 0, \"othersecondarycardweight\": 0, \"othercardweight\": 0}, \"2\": {\"jokercardweight\": 10, \"keycardweight\": 5, \"mainsecondarycardweight\": 0, \"othersecondarycardweight\": 0, \"othercardweight\": 0}, \"3\": {\"jokercardweight\": 0, \"keycardweight\": 1, \"mainsecondarycardweight\": 0, \"othersecondarycardweight\": 0, \"othercardweight\": 0}, \"4\": {\"jokercardweight\": 0, \"keycardweight\": 1, \"mainsecondarycardweight\": 0, \"othersecondarycardweight\": 0, \"othercardweight\": 0}, \"5\": {\"jokercardweight\": 0, \"keycardweight\": 1, \"mainsecondarycardweight\": 0, \"othersecondarycardweight\": 0, \"othercardweight\": 0}, \"6\": {\"jokercardweight\": 0, \"keycardweight\": 10, \"mainsecondarycardweight\": 5, \"othersecondarycardweight\": 0, \"othercardweight\": 0}, \"7\": {\"jokercardweight\": 0, \"keycardweight\": 5, \"mainsecondarycardweight\": 5, \"othersecondarycardweight\": 0, \"othercardweight\": 15}}, \"drawcardweightinfo\": {\"1\": {\"jokercardweight\": 2, \"keycardweight\": 20, \"mainsecondarycardweight\": 10, \"othersecondarycardweight\": 0, \"othercardweight\": 20}, \"2\": {\"jokercardweight\": 4, \"keycardweight\": 20, \"mainsecondarycardweight\": 15, \"othersecondarycardweight\": 0, \"othercardweight\": 20}, \"3\": {\"jokercardweight\": 6, \"keycardweight\": 200, \"mainsecondarycardweight\": 10, \"othersecondarycardweight\": 0, \"othercardweight\": 50}, \"4\": {\"jokercardweight\": 8, \"keycardweight\": 100, \"mainsecondarycardweight\": 20, \"othersecondarycardweight\": 5, \"othercardweight\": 30}, \"5\": {\"jokercardweight\": 10, \"keycardweight\": 14, \"mainsecondarycardweight\": 19, \"othersecondarycardweight\": 0, \"othercardweight\": 50}, \"6\": {\"jokercardweight\": 12, \"keycardweight\": 16, \"mainsecondarycardweight\": 16, \"othersecondarycardweight\": 5, \"othercardweight\": 50}, \"7\": {\"jokercardweight\": 15, \"keycardweight\": 35, \"mainsecondarycardweight\": 20, \"othersecondarycardweight\": 10, \"othercardweight\": 25}, \"8\": {\"jokercardweight\": 12, \"keycardweight\": 10, \"mainsecondarycardweight\": 35, \"othersecondarycardweight\": 15, \"othercardweight\": 30}, \"9\": {\"jokercardweight\": 12, \"keycardweight\": 10, \"mainsecondarycardweight\": 35, \"othersecondarycardweight\": 15, \"othercardweight\": 30}, \"10\": {\"jokercardweight\": 12, \"keycardweight\": 10, \"mainsecondarycardweight\": 35, \"othersecondarycardweight\": 15, \"othercardweight\": 30}, \"11\": {\"jokercardweight\": 12, \"keycardweight\": 10, \"mainsecondarycardweight\": 35, \"othersecondarycardweight\": 15, \"othercardweight\": 30}, \"12\": {\"jokercardweight\": 12, \"keycardweight\": 10, \"mainsecondarycardweight\": 35, \"othersecondarycardweight\": 15, \"othercardweight\": 30}, \"13\": {\"jokercardweight\": 12, \"keycardweight\": 10, \"mainsecondarycardweight\": 35, \"othersecondarycardweight\": 15, \"othercardweight\": 30}, \"14\": {\"jokercardweight\": 10, \"keycardweight\": 10, \"mainsecondarycardweight\": 35, \"othersecondarycardweight\": 15, \"othercardweight\": 30}, \"15\": {\"jokercardweight\": 10, \"keycardweight\": 10, \"mainsecondarycardweight\": 35, \"othersecondarycardweight\": 15, \"othercardweight\": 30}, \"16\": {\"jokercardweight\": 4, \"keycardweight\": 15, \"mainsecondarycardweight\": 20, \"othersecondarycardweight\": 35, \"othercardweight\": 20}, \"17\": {\"jokercardweight\": 10, \"keycardweight\": 10, \"mainsecondarycardweight\": 35, \"othersecondarycardweight\": 15, \"othercardweight\": 30}, \"18\": {\"jokercardweight\": 10, \"keycardweight\": 10, \"mainsecondarycardweight\": 35, \"othersecondarycardweight\": 15, \"othercardweight\": 30}, \"19\": {\"jokercardweight\": 10, \"keycardweight\": 10, \"mainsecondarycardweight\": 35, \"othersecondarycardweight\": 15, \"othercardweight\": 30}, \"20\": {\"jokercardweight\": 10, \"keycardweight\": 10, \"mainsecondarycardweight\": 35, \"othersecondarycardweight\": 15, \"othercardweight\": 30}, \"-1\": {\"jokercardweight\": 10, \"keycardweight\": 10, \"mainsecondarycardweight\": 35, \"othersecondarycardweight\": 15, \"othercardweight\": 30}}, \"dealsuitweight\": {\"samesuit\": 0, \"samecolor\": 10, \"randsuit\": 10, \"varysuitall\": 0, \"varysuitdeal\": 5}, \"maxkeycardcnt\": {\"min\": 8, \"max\": 15}, \"startgoodkeycardcnt\": {\"min\": 6, \"max\": 6}, \"expectkeycardcnt\": {\"min\": 9, \"max\": 9}, \"maxjokercardcnt\": {\"min\": 3, \"max\": 6}, \"startjokercnt\": {\"min\": 7, \"max\": 7}, \"limitjokercnt\": {\"min\": 9, \"max\": 9}, \"expectjokercnt\": {\"min\": 2, \"max\": 2}, \"drawmaincarddelweightinfo\": {\"1\": 20, \"2\": 20, \"3\": 10, \"4\": 5, \"5\": 0, \"6\": 0, \"-1\": 0}, \"continuedrawkeycardinfo\": {\"1\": {\"succ\": 8, \"fail\": 2}, \"2\": {\"succ\": 5, \"fail\": 5}, \"3\": {\"succ\": 6, \"fail\": 4}, \"4\": {\"succ\": 4, \"fail\": 6}, \"5\": {\"succ\": 3, \"fail\": 8}, \"6\": {\"succ\": 0, \"fail\": 9}, \"7\": {\"succ\": 0, \"fail\": 1}, \"8\": {\"succ\": 0, \"fail\": 1}, \"9\": {\"succ\": 0, \"fail\": 1}, \"10\": {\"succ\": 0, \"fail\": 1}, \"-1\": {\"succ\": 0, \"fail\": 1}}}";
    
        mTempData[1] = oConfig;
    }

    return mTempData;
}

const std::unordered_map<int, SHeroConfig> CHeroConfig::mData = CHeroConfig::_InitData();

const SHeroConfig* CHeroConfig::Get(int nId) {
    std::unordered_map<int, SHeroConfig>::const_iterator it = mData.find(nId);
    if (it != mData.end()) {
        return &it->second;
    }
    return nullptr;
}