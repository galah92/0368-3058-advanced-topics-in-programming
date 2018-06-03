#pragma once

#include <unordered_map>
#include <functional>
#include <utility>
#include <string>
#include <atomic>
#include <memory>
#include <deque>
#include <mutex>
#include <map>
#include "PlayerAlgorithm.h"


class TournamentManager {
public:
    static TournamentManager& get();
    void registerAlgorithm(std::string id, std::function<std::unique_ptr<PlayerAlgorithm>()> factoryMethod);
    void run();
    unsigned int maxThreads;
    std::string path;
private:
    void worker();
    void output();
    static TournamentManager _singleton;
    TournamentManager() = default;
    std::unordered_map<std::string, std::function<std::unique_ptr<PlayerAlgorithm>()>> _algorithms;
    std::unordered_map<std::string, std::shared_ptr<std::atomic<unsigned int>>> _scores;
    std::deque<std::pair<std::string, std::string>> _games;
    std::mutex _gamesMutex;
    const static unsigned int MAX_GAME = 30;
};