#include <algorithm>
#include <iostream>
#include <random>
#include <dlfcn.h>
// #include <sys/types.h>
// #include <dirent.h>
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
    _numGames[id] = 0;
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

void TournamentManager::workerThread() {
    GameManager gameManager;
    while (true) {
        _scoresMutex.lock();
        if (_games.empty()) { // no games left
            _scoresMutex.unlock();
            return;        
        }
        auto idsPair = _games.front();
        // idsPair is pair of pair<string, bool>
        _games.pop_front();
        _scoresMutex.unlock();
        auto winner = gameManager.playRound(_algos[idsPair.first.first](), _algos[idsPair.second.first]());
        std::lock_guard<std::mutex> guard(_scoresMutex);
        if (winner == 1 && idsPair.first.second == true) {
            _scores[idsPair.first.first] += 3;
        } else if (winner == 2 && idsPair.second.second == true) {
            _scores[idsPair.second.first] += 3;
        } else { // tie
            if(idsPair.first.second == true) _scores[idsPair.first.first]++;
            if(idsPair.second.second == true) _scores[idsPair.second.first]++;
        }
    }
}

bool TournamentManager::isValidLib(const std::string fname) const {
    if (fname.find("RSPPlayer_") != 0) return false; // wrong prefix
    const std::string ext = ".so";
    if (fname.compare(fname.length() - ext.length(), ext.length(), ext)) return false;
	return true;
}

void TournamentManager::loadSharedLibs() {
    // DIR* dir = opendir(path.c_str());
    // if (!dir) return;
	// struct dirent* ep;
	// while ((ep = readdir(dir))) {
	// 	if (isValidLib(ep->d_name)) {
	// 		std::string file(path + std::string("/") + std::string(ep->d_name));
	// 		void* lib = dlopen(file.c_str(), RTLD_LAZY);
    //         if (lib) {
    //             _libs.push_back(lib);
    //         } else {
    //             std::cout << "Error loading" << dlerror() << std::endl;
    //         }
	// 	}
	// }
	// closedir(dir);
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

std::pair<int, int> chooseTwoGames(const std::vector<std::pair<std::string, int>>& games){
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
    std::vector<std::pair<std::string, int>> currentGames;
    for (const auto& algo : _algos) currentGames.emplace_back(algo.first, 30);
    while (currentGames.size() > 1) {
        auto gamesIndices = chooseTwoGames(currentGames);
        // push valid game(two algo's) to _games
        _games.emplace_back(std::make_pair(currentGames[gamesIndices.first].first, true), std::make_pair(currentGames[gamesIndices.second].first, true)); 
        currentGames[gamesIndices.first].second--;
        currentGames[gamesIndices.second].second--;
        // remove algo's which complete 30 games
        if (currentGames[gamesIndices.first].second == 0) currentGames.erase(currentGames.begin() + gamesIndices.first);
        if (currentGames[gamesIndices.second].second == 0) currentGames.erase(currentGames.begin() + gamesIndices.second);
    }
    if (currentGames.size() == 0) return; // there is one algo in currentGames
    auto algo = currentGames.back();
    // find opponent
    std::string opponent;
    for (const auto &op : _algos) {
        if (op.first != algo.first) {
            opponent = op.first;
            break;
        }
    }
    while (algo.second > 0) {
        _games.emplace_back(std::make_pair(algo.first, true),std::make_pair(opponent, false));
        algo.second--;
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