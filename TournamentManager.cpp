#include <iostream>
#include <algorithm>
#include <iterator>
#include <thread>
#include <experimental/filesystem>
#include "TournamentManager.h"
#include "GameManager.h"

#if defined(__linux__) || defined(__APPLE__)
#include <dlfcn.h>
#elif _WIN32
#include <Windows.h>
#endif


class SharedLib {
public:
    SharedLib(const std::experimental::filesystem::v1::path& path) {
#if defined(__linux__) || defined(__APPLE__)
        _lib = dlopen(path.c_str(), RTLD_LAZY);
#elif _WIN32
        _lib = LoadLibrary(path.string().c_str());
#endif
    }
    ~SharedLib() {
        if (!_lib) return;
#if defined(__linux__) || defined(__APPLE__)
        dlclose(_lib);
#elif _WIN32
        FreeLibrary((HMODULE)_lib); // TODO: change to dynamic_cast
#endif
        _lib = nullptr;
    }
    operator bool() const { return _lib; }
    static bool valid(const std::experimental::filesystem::v1::path& path) {
#if defined(__linux__) || defined(__APPLE__)
        return path.extension() == ".so";
#elif _WIN32
        return path.extension() == ".dll";
#endif
        return true;
    }
private:
    void* _lib = nullptr;
};

TournamentManager TournamentManager::_singleton;

TournamentManager& TournamentManager::get() {
    return _singleton;
}

void TournamentManager::registerAlgorithm(std::string id, std::function<std::unique_ptr<PlayerAlgorithm>()> factoryMethod) {
    _algorithms[id] = factoryMethod;
    _scores.emplace(id, std::make_shared<std::atomic<unsigned int>>(0));
}

void TournamentManager::run() {
    const auto sharedLibs = loadSharedLibs(); // registering all algorithms
    if (sharedLibs.empty()) {
        std::cout << "Error: no shared libraries loaded" << std::endl;
        return;
    }
    initGames();
    // init all worker threads
    std::vector<std::thread> threads;
    for (auto i = 0; i < maxThreads - 1; i++) {
        threads.emplace_back(&TournamentManager::workerThread, this);
    }
    workerThread(); // main thread should also participate
    for (auto& thread : threads) thread.join();
    output();
}

std::vector<SharedLib> TournamentManager::loadSharedLibs() {
    std::vector<SharedLib> libs;
    if (!std::experimental::filesystem::is_directory(path)) return libs;
    const auto& it = std::experimental::filesystem::directory_iterator(path);
    for (const auto& file : it) {
        const auto& fpath = file.path();
        if (!SharedLib::valid(fpath)) continue;
        SharedLib lib(fpath);
        if (lib) {
            libs.push_back(lib);
        } else {
            std::cout << "Error: failed loading " << fpath.string() << std::endl;
        }
    }
    return libs;
}
std::pair<int, int> chooseTwoGames(const std::unique_ptr<std::vector<std::pair<std::string, int>>> &cgPointer){
    //TODO : implement
    (void) cgPointer;
    return std::make_pair(0,0);
}

void TournamentManager::initGames() {
    _games.clear();
    std::vector<std::pair<std::string,int>> currentGames;
    for (const auto &algo : _algorithms){ // initialize every algorithm to 30 games
        currentGames.push_back(std::make_pair(algo.first, 30));
    }
    auto cgPointer = std::make_unique<std::vector<std::pair<std::string, int>>>(currentGames);
    while(currentGames.size() > 1){
        auto gamesIndices = chooseTwoGames(cgPointer);
        // push valid game(two algo's) to _games
        _games.emplace_back(std::make_pair(currentGames[gamesIndices.first].first, true),
                            std::make_pair(currentGames[gamesIndices.second].first, true)); 
        currentGames[gamesIndices.first].second--;
        currentGames[gamesIndices.second].second--;
        if (currentGames[gamesIndices.first].second == 0) currentGames.erase(currentGames.begin() + gamesIndices.first);
        if (currentGames[gamesIndices.second].second == 0) currentGames.erase(currentGames.begin() + gamesIndices.second);
    }
    if (currentGames.size() == 0) return;
    // there is one algo in currentGames
    // while()
}

void TournamentManager::workerThread() {
    GameManager gameManager;
    while (true) {
        _gamesMutex.lock();
        if (_games.empty()) { // no games left
            _gamesMutex.unlock();
            return;        
        }
        auto idsPair = _games.front();
        // idsPair is pair of pair<string, bool>
        auto ids = _games.front();
        _games.pop_front();
        _gamesMutex.unlock();
        auto winner = gameManager.playRound(_algorithms[idsPair.first.first](), _algorithms[idsPair.second.first]());
        if (winner == 1 && idsPair.first.second == true) {
            _scores[idsPair.first.first]->operator+=(3);
        } else if (winner == 2 && idsPair.second.second == true) {
            _scores[idsPair.second.first]->operator+=(3);
        } else { // tie
            if(idsPair.first.second == true) _scores[idsPair.first.first]->operator++();
            if(idsPair.second.second == true) _scores[idsPair.second.first]->operator++();
        }
    }
}

void TournamentManager::output() {
    std::vector<std::pair<std::string, std::shared_ptr<std::atomic<unsigned int>>>> vec;
    copy(_scores.begin(), _scores.end(), std::back_inserter(vec));
    std::sort(vec.begin(), vec.end(), [](const auto& lhs, const auto& rhs) {
        return lhs.second < rhs.second; // sort by score
    });
    for (const auto& pair : vec) {
        std::cout << pair.first << " " << *pair.second << std::endl;
    }
}