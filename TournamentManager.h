#pragma once

#include <unordered_map>
#include <functional>
#include <utility>
#include <string>
#include <atomic>
#include <memory>
#include <random>
#include <deque>
#include <mutex>
#include <map>
#include "PlayerAlgorithm.h"


class SharedLib;

class TournamentManager {
public:
    static TournamentManager& get();
    void registerAlgorithm(std::string id, std::function<std::unique_ptr<PlayerAlgorithm>()> factoryMethod);
    void run();
    int maxThreads = 4;
    std::string path = "./";
private:
    TournamentManager() = default;
    std::vector<SharedLib> loadSharedLibs();
    void initGames();
    void workerThread();
    void output();
    static TournamentManager _singleton;
    std::unordered_map<std::string, std::function<std::unique_ptr<PlayerAlgorithm>()>> _algorithms;
    std::unordered_map<std::string, std::shared_ptr<std::atomic<unsigned int>>> _scores;
    std::deque<std::pair<std::pair<std::string, bool>, std::pair<std::string, bool>>> _games;
    std::mutex _gamesMutex;
    const static unsigned int MAX_GAMES = 30;
};