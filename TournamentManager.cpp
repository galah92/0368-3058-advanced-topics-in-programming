#include <iostream>
#include <algorithm>
#include <iterator>
#include <thread>
#include <utility>
#include "TournamentManager.h"


TournamentManager TournamentManager::_singleton;

TournamentManager& TournamentManager::get() {
    return _singleton;
}

void TournamentManager::registerAlgorithm(std::string id, std::function<std::unique_ptr<PlayerAlgorithm>()> factoryMethod) {
    _algorithms[id] = factoryMethod;
    _scores.emplace(id, std::make_shared<std::atomic<unsigned int>>(0));
}

void TournamentManager::run() {
    // TODO: load all alogs
    // TODO: create deque of games to play
    // init all worker threads
    std::vector<std::thread> threads;
    for (unsigned int i = 0; i < maxThreads - 1; i++) {
        threads.emplace_back(&TournamentManager::worker, this);
    }
    worker(); // main thread should also participate
    // join all threads
    std::for_each(threads.begin(), threads.end(), [](auto& t) { t.join(); });
    output();
}

void TournamentManager::worker() {
    for (const auto& pair : _algorithms) {
        std::cout << pair.first << std::endl;
    }
}

void TournamentManager::output() {
    std::vector<std::pair<std::string, std::shared_ptr<std::atomic<unsigned int>>>> vec;
    copy(_scores.begin(), _scores.end(), std::back_inserter(vec));
    std::sort(vec.begin(), vec.end(), [](const auto& lhs, const auto& rhs) {
        return lhs.second < rhs.second;
    });
    std::for_each(vec.begin(), vec.end(), [](const auto& pair) {
        std::cout << pair.first << " " << *pair.second << std::endl;
    });
}