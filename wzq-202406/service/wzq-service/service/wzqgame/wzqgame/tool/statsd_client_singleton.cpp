#include "statsd_client_singleton.hpp"

Statsd::StatsdClient& StatsdClientSingleton::getInstance() {
    static Statsd::StatsdClient instance("8.130.49.219", 8125, "game");
    return instance;
}
