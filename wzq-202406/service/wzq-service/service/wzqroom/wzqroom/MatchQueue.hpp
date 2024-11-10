//
//  MatchQueue.hpp
//  wzqroom
//
//  Created by duole-43 on 2024/7/24.
//

#ifndef MatchQueue_hpp
#define MatchQueue_hpp

#include <iostream>
#include <list>
#include <unordered_map>
#include <google/protobuf/stubs/common.h>
#include "log.h"

using google::protobuf::int64;
using namespace std;

class MatchQueue {
public:
    void push(int64 userId);
    
    void remove(int64 userId);
    
    size_t size() const;
    
    bool empty() const;
    
    int64 front() const;
    
    void pop();
    
    void print() const;
private:
    list<int64> playerList;
    unordered_map<int64, list<int64>::iterator> playerMap;
};

#endif /* MatchQueue_hpp */
