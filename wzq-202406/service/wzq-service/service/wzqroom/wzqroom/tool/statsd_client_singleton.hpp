#ifndef STATSD_CLIENT_SINGLETON_HPP
#define STATSD_CLIENT_SINGLETON_HPP

#include <cpp-statsd-client/StatsdClient.hpp>

class StatsdClientSingleton {
public:
    static Statsd::StatsdClient& getInstance();
    
private:
    StatsdClientSingleton() = default;
    ~StatsdClientSingleton() = default;
    StatsdClientSingleton(const StatsdClientSingleton&) = delete;
    StatsdClientSingleton& operator=(const StatsdClientSingleton&) = delete;
};

#endif // STATSD_CLIENT_SINGLETON_HPP
