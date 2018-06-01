#include "TournamentManager.h"


TournamentManager TournamentManager::_instance;

TournamentManager& TournamentManager::getInstance() {
    return _instance;
}

void TournamentManager::registerAlgorithm(std::string id, std::function<std::unique_ptr<PlayerAlgorithm>()> factoryMethod) {
    if (algos.find(id) == algos.end()) {
        // TODO: error - algo already exist!
    }
    algos[id] = factoryMethod;
}