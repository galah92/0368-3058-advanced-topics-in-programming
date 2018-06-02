#pragma once

#include <functional>
#include <memory>
#include <map>
#include <unordered_map>
#include "PlayerAlgorithm.h"


class TournamentManager {
public:
    static TournamentManager& get();
    void registerAlgorithm(std::string id, std::function<std::unique_ptr<PlayerAlgorithm>()> factoryMethod);
    void run();
private:
    static TournamentManager _singleton;
    TournamentManager() = default;
    std::unordered_map<std::string, std::function<std::unique_ptr<PlayerAlgorithm>()>> id2factory;
};