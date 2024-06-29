#ifndef ROUTERFACTORY_H
#define ROUTERFACTORY_H

#include <unordered_map>
#include <functional>
#include <string>
#include "Router.h"

class RouterFactory {
public:
    // 使用lambda来实现延迟构建对象， 并且创建对象的函数中还可以做一些别的事情，更易拓展
    using CreateRouterFunc = std::function<Router*()>;

    static RouterFactory& instance() {
        static RouterFactory instance;
        return instance;
    }

    void registerRouter(const std::string& commandID, CreateRouterFunc func) {
        routerMap[commandID] = func;
    }

    Router* createRouter(const std::string& commandID) {
        auto it = routerMap.find(commandID);
        if (it != routerMap.end()) {
            return it->second();
        }
        return nullptr;
    }

private:
    RouterFactory() {}
    std::unordered_map<std::string, CreateRouterFunc> routerMap;
};


#define REGISTER_HANDLE(commandID, RouterType) \
    static bool dummy_##RouterType = []() { \
        RouterFactory::instance().registerRouter(commandID, []() -> Router* { return new RouterType(); }); \
        return true; \
    }()

#endif 
