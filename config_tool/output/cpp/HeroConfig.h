#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <utility> // For std::pair

// 此文件由 tools/generate.py 自动生成，请勿手动修改。

struct SHeroConfig {

    int nId;

    std::string sName;

    float fValueFloat;

    std::vector<int> vTestint;

    std::vector<std::string> vKeywords;

    std::unordered_map<std::string, int> mStrToIntMap;

    std::vector<std::pair<std::string, int> > vStrPairArray;

    std::string sComplexLogic;

    std::string sTestjson;

};

class CHeroConfig {
public:
    static const std::unordered_map<int, SHeroConfig> mData;
    static const SHeroConfig* Get(int nId);

private:
    static std::unordered_map<int, SHeroConfig> _InitData();
};