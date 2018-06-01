#pragma once

#include <functional>
#include <memory>
#include <map>
#include "PlayerAlgorithm.h"


class TournamentManager {
public:
    static TournamentManager& getInstance();
    void registerAlgorithm(std::string id, std::function<std::unique_ptr<PlayerAlgorithm>()> factoryMethod);
    void run();
private:
    TournamentManager() {};
    static TournamentManager _instance;
    std::map<std::string, std::function<std::unique_ptr<PlayerAlgorithm>()>> algos;
};