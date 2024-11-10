//
//  MatchQueue.cpp
//  wzqroom
//
//  Created by duole-43 on 2024/7/24.
//

#include "MatchQueue.hpp"

void MatchQueue::push(int64 userId) {
    // todo 去重，后续需在用户断连时通知room服务器移除正在匹配的id
    if (playerMap.find(userId) != playerMap.end()) {
        return;
    }
    
    playerList.push_back(userId);
    playerMap[userId] = --playerList.end();
}

void MatchQueue::remove(int64 userId) {
    auto it = playerMap.find(userId);
    if (it != playerMap.end()) {
        playerList.erase(it->second);
        playerMap.erase(it);
    }
}

size_t MatchQueue::size() const {
    return playerList.size();
}

bool MatchQueue::empty() const {
    return playerList.empty();
}

int64 MatchQueue::front() const {
    return playerList.front();
}

void MatchQueue::pop() {
    int64 userId = playerList.front();
    playerList.pop_front();
    playerMap.erase(userId);
}

void MatchQueue::print() const {
    LOG("MatchQueue", "print", "当前匹配队列有%d人", size());
    int index = 1;
    for (const auto& userId : playerList) {
        LOG("MatchQueue", "print", "待匹配第%d位：%d", index++, userId);
    }
}
