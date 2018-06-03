#include <iostream>
#include <algorithm>
#include <iterator>
#include <thread>
#include <experimental/filesystem>
#include "TournamentManager.h"
#include "GameManager.h"

#ifdef __linux__
#include <dlfcn.h>
#endif // __linux__


class shared_lib {
public:
    shared_lib(const std::experimental::filesystem::v1::path& path) {
#ifdef __linux__
        if (path.extension() != ".so") return;
        _lib = dlopen(path.c_str(), RTLD_LAZY);
#endif // __linux__
    }
    ~shared_lib() {
#ifdef __linux__
        if (_lib) dlclose(_lib);
#endif // __linux__
        _lib = nullptr;
    }
    shared_lib(const shared_lib& lib) = default;
    shared_lib(shared_lib&& lib) = default;
    operator bool() const { return _lib; }
    static bool valid(const std::experimental::filesystem::v1::path& path) {
#ifdef __linux__
        return path.extension() == ".so";
#endif // __linux__
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

std::vector<shared_lib> TournamentManager::loadSharedLibs() {
    std::vector<shared_lib> libs;
    const auto& it = std::experimental::filesystem::directory_iterator(path);
    for (const auto& file : it) {
        const auto& fpath = file.path();
        if (!shared_lib::valid(fpath)) continue;
        shared_lib lib(fpath);
        if (lib) {
            libs.push_back(lib);
        } else {
            std::cout << "Error loading " << fpath.string() << std::endl;
        }
    }
    return libs;
}

void TournamentManager::initGames() {
    // TODO: populate _games
}

void TournamentManager::workerThread() {
    GameManager gameManager;
    while (true) {
        _gamesMutex.lock();
        if (!_games.size()) break; // no games left
        auto ids = _games.front();
        _games.pop_front();
        _gamesMutex.unlock();
        auto winner = gameManager.playRound(_algorithms[ids.first](), _algorithms[ids.second]());
        if (winner == 1) {
            _scores[ids.first]->operator+=(3);
        } else if (winner == 2) {
            _scores[ids.second]->operator+=(3);
        } else { // tie
            _scores[ids.first]->operator++();
            _scores[ids.second]->operator++();
        }
    }
    _gamesMutex.unlock(); // cause we're still aquiring it
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