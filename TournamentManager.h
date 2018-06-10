#pragma once

#include <functional>
#include <string>
#include <thread>
#include <mutex>
#include <deque>
#include <map>
#include "PlayerAlgorithm.h"


class TournamentManager {
public:
    static TournamentManager& getTournamentManager() { return _singleton; }
    TournamentManager(const TournamentManager&) = delete;
    TournamentManager& operator=(const TournamentManager&) = delete;
    void registerAlgorithm(std::string id, std::function<std::unique_ptr<PlayerAlgorithm>()> factoryMethod);
    void run();
    unsigned int maxThreads = 4;
    std::string path = "./";
private:
    TournamentManager() = default;
    bool isValidLib(const std::string fname) const;
    void loadSharedLibs();
    void freeSharedLibs();
    void initGames();
    void workerThread();
    void output() const;
    static TournamentManager _singleton;
    std::map<std::string, std::function<std::unique_ptr<PlayerAlgorithm>()>> _algos;
    std::map<std::string, unsigned int> _scores;
    std::map<std::string, unsigned int> _numGames;
    std::deque<std::pair<std::pair<std::string, bool>, std::pair<std::string, bool>>> _games;
    std::vector<void *> _libs;
    std::mutex _scoresMutex;
    std::mutex _numGamesMutex;
    const static unsigned int _MAX_GAMES = 30;
};