#include <algorithm>
#include <iostream>
#include <random>
#include <dlfcn.h>
#include <experimental/filesystem>
#include "TournamentManager.h"
#include "GameManager.h"
#include "AlgorithmRegistration.h"


AlgorithmRegistration::AlgorithmRegistration(std::string id, std::function<std::unique_ptr<PlayerAlgorithm>()> factoryMethod) {
	TournamentManager::getTournamentManager().registerAlgorithm(id, factoryMethod);
}

TournamentManager TournamentManager::_singleton;

void TournamentManager::registerAlgorithm(std::string id, std::function<std::unique_ptr<PlayerAlgorithm>()> factoryMethod) {
	if(_algos.find(id) != _algos.end()) {
        std::cout << "ERROR: " << id << " is registered, skipping" << std::endl;
	}
	_algos[id] = factoryMethod;
	_scores[id] = 0;
}

void TournamentManager::run() {
    loadSharedLibs();
    if (_algos.size() < 2) return; // not enough players
    initGames();
    // init all worker threads
    std::vector<std::thread> threads;
    for (unsigned int i = 0; i < maxThreads - 1; i++) {
        threads.emplace_back(&TournamentManager::workerThread, this);
    }
    workerThread(); // main thread should also participate
    for (auto& thread : threads) thread.join();
    output();
    freeSharedLibs();
}

bool TournamentManager::isValidLib(const std::string fname) const {
    if (fname.find("RSPPlayer_") != 0) return false; // wrong prefix
    const std::string ext = ".so";
    if (fname.compare(fname.length() - ext.length(), ext.length(), ext)) return false;
	return true;
}

void TournamentManager::loadSharedLibs() {
    namespace fs = std::experimental::filesystem::v1;
    if (!fs::is_directory(path)) return;
    for (const auto& file : fs::directory_iterator(path)) {
        if (file.path().extension() != ".so") continue;
        if (file.path().string().find("RSPPlayer_") == 0) continue;
        void* lib = dlopen(file.path().c_str(), RTLD_LAZY);
        if (lib) {
            _libs.push_back(lib);
        } else {
            std::cout << "Error loading " << dlerror() << std::endl;
        }
    }
}

void TournamentManager::freeSharedLibs() {
    _algos.clear();
    for (const auto& lib : _libs) dlclose(lib);
}

std::pair<int, int> chooseTwoGames(const std::vector<std::pair<std::string, unsigned int>>& games){
    auto _rg = std::mt19937(std::random_device{}());
    auto n = std::uniform_int_distribution<int>(0, games.size() - 1)(_rg);
    auto k = std::uniform_int_distribution<int>(0, games.size() - 1)(_rg);
    while (n == k) {
        k = std::uniform_int_distribution<int>(0, games.size() - 1)(_rg);
    }
    return std::make_pair(n,k);
}

void TournamentManager::initGames() {
    _games.clear();
    std::vector<std::pair<std::string, unsigned int>> numGames;
    for (auto& algo : _algos) numGames.emplace_back(algo.first, _MAX_GAMES);
    while (numGames.size() > 1) {
        auto indices = chooseTwoGames(numGames);
        // push valid game(two algo's) to _games
        _games.emplace_back(numGames[indices.first].first, numGames[indices.second].first, true);
        numGames[indices.first].second--;
        numGames[indices.second].second--;
        // remove algo's which complete 30 games
        if (numGames[indices.first].second == 0) numGames.erase(numGames.begin() + indices.first);
        if (numGames[indices.second].second == 0) numGames.erase(numGames.begin() + indices.second);
    }
    if (numGames.size() == 0) return; // there is one algo in numGames
    auto algo = numGames.back();
    // find opponent
    std::string opponent;
    for (const auto &op : _algos) {
        if (op.first != algo.first) {
            opponent = op.first;
            break;
        }
    }
    while (algo.second > 0) {
        _games.emplace_back(algo.first, opponent, false);
        algo.second--;
    }
}

void TournamentManager::workerThread() {
    GameManager gameManager;
    while (true) {
        std::unique_lock<std::mutex> lock(_scoresMutex);
        if (_games.empty()) return; // no games left
        auto match = _games.front();
        _games.pop_front();
        lock.unlock(); // allow other threads to deque for matches
        const auto& id1 = std::get<0>(match);
        const auto& id2 = std::get<1>(match);
        bool toUpdateScore = std::get<2>(match);
        auto winner = gameManager.playRound(_algos[id1](), _algos[id2]());
        // std::cout << "================ winner: " << (winner == 0 ? "tie" : (winner == 1 ? id1 : id2)) << std::endl;
        if (winner == 1) {
            _scores[id1] += 3;
        } else if (winner == 2 && toUpdateScore) {
            _scores[id2] += 3;
        } else { // tie
            _scores[id1]++;
            _scores[id2]++;
        }
    }
}

void TournamentManager::output() const {
    std::vector<std::pair<std::string, unsigned int>> vec;
    for (const auto& p : _scores) vec.push_back(p);
    std::sort(vec.begin(), vec.end(), [](const auto& p1, const auto& p2) {
        return p1.second > p2.second;
    });
    for (const auto& p : vec) {
        std::cout << p.first << " " << p.second << std::endl;
    }
}