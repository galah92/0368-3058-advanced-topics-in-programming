#include "TournamentManager.h"


TournamentManager TournamentManager::_singleton;

TournamentManager& TournamentManager::get() {
    return _singleton;
}

void TournamentManager::registerAlgorithm(std::string id, std::function<std::unique_ptr<PlayerAlgorithm>()> factoryMethod) {
    id2factory[id] = factoryMethod;
}