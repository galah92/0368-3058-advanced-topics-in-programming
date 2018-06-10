#include <algorithm>
#include <iostream>
#include <random>
#include <dlfcn.h>
#include <sys/types.h>
#include <dirent.h>
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
    playerAllGames();
    output();
    freeSharedLibs();
}

bool TournamentManager::isValidLib(const char* fileName) {
	std::string filename(fileName);
	std::string start("RSPPlayer_");
	std::string end(".so");
	if (filename.length() != 22) return false;
	std::string fileStart(filename.substr(0, start.length()));
	if (fileStart.compare(start) != 0) return false;
	std::string fileEnd(filename.substr(start.length()+9, 22));
	if (fileEnd.compare(end) != 0) return false;
	return true;
}

void TournamentManager::loadSharedLibs() {
    DIR* dir = opendir(path.c_str());
    if (!dir) return;
	struct dirent* ep;
	while ((ep = readdir(dir))) {
		if (isValidLib(ep->d_name)) {
			std::string file(path + std::string("/") + std::string(ep->d_name));
			void* lib = dlopen(file.c_str(), RTLD_LAZY);
            if (lib) {
                _libs.push_back(lib);
            } else {
                std::cout << "Error loading " << file << ": " << dlerror() << std::endl;
            }
		}
	}
	closedir(dir);
    // namespace fs = std::experimental::filesystem::v1;
    // if (!fs::is_directory(path)) return;
    // for (const auto& file : fs::directory_iterator(path)) {
    //     if (file.path().extension() != ".so") continue;
    //     void* lib = dlopen(file.path().c_str(), RTLD_LAZY);
    //     if (lib) {
    //         _libs.push_back(lib);
    //     } else {
    //         std::cout << "Error loading " << file.path() << ": " << dlerror() << std::endl;
    //     }
    // }
}

void TournamentManager::freeSharedLibs() {
    _algos.clear();
    for (const auto& lib : _libs) dlclose(lib);
}

void TournamentManager::playerAllGames() {
    std::vector<std::thread> threads;
    for (unsigned int i = 0; i < maxThreads - 1; i++) {
        threads.emplace_back(&TournamentManager::gamesWorker, this, i);
    }
    gamesWorker(maxThreads - 1); // main thread should also participate
    for (auto& t : threads) t.join();
    // handle leftover algorithms
    std::string leftPlayer;
    if(_numGames.size() > 0) { // assuming _numGames.size() == 1
        leftPlayer = _numGames.begin()->first;
        auto it = _scores.begin();
        while (_numGames[leftPlayer] < _MAX_GAMES) {
            // do not play against yourself
            if (leftPlayer.compare(it->first) != 0) {
                runGameBetweenTwoPlayers(leftPlayer,it->first,false);
                _numGames[leftPlayer]++;
            }
            it++;
            // If we reached the end of the map - start over
            if (it == _scores.end()) it = _scores.begin();
        }
    }
}

void TournamentManager::runGameBetweenTwoPlayers(std::string firstPlayerID, std::string secondPlayerID, bool updateSecondPlayer) {
	GameManager game;
    auto winner = game.playRound(_algos[firstPlayerID](), _algos[secondPlayerID]());
    std::lock_guard<std::mutex> guard(_scoresMutex);
    if (winner == 1) {
        _scores[firstPlayerID] += 3;
    } else if (winner == 2 && updateSecondPlayer) {
        _scores[secondPlayerID] += 3;
    } else {
        _scores[firstPlayerID] += 1;
        if(updateSecondPlayer) {
            _scores[secondPlayerID] += 1;
        }
    }
}

void TournamentManager::gamesWorker(int seedNum) {
    std::string firstPlayer;
    std::string  secondPlayer;
    // For random numbers
    std::default_random_engine generator(seedNum * 10);
    std::uniform_int_distribution<int> distribution(0, _numGames.size());
    GameManager game;
    while (true) {
        // Check if there is enough players
        _numGamesMutex.lock();
        if(_numGames.size() <= 1) {
            _numGamesMutex.unlock();
            return;
        }
        _numGamesMutex.unlock();
        firstPlayer = "";
        secondPlayer = "";
        while (firstPlayer.compare(secondPlayer) == 0) {
            firstPlayer = getPlayerId(distribution(generator) % _numGames.size());
            secondPlayer = getPlayerId(distribution(generator) % _numGames.size());
        }
        _numGamesMutex.lock();
        _numGames[firstPlayer]++;
        _numGames[secondPlayer]++;
        if(_numGames[firstPlayer] == _MAX_GAMES) _numGames.erase(firstPlayer);
        if(_numGames[secondPlayer] == _MAX_GAMES) _numGames.erase(secondPlayer);
        _numGamesMutex.unlock();
        auto winner = game.playRound(_algos[firstPlayer](), _algos[secondPlayer]());
        std::lock_guard<std::mutex> guard(_scoresMutex);
        if (winner == 1) {
            _scores[firstPlayer] += 3;
        } else if (winner == 2 && true) {
            _scores[firstPlayer] += 3;
        } else {
            _scores[firstPlayer] += 1;
            if (true) {
                _scores[secondPlayer] += 1;
            }
        }

    }
}

const std::string& TournamentManager::getPlayerId(int randNum) {
    auto randomIter = _numGames.begin();
    if ((unsigned int)randNum > _numGames.size()) return randomIter->first;
    std::advance(randomIter, randNum);
    return randomIter->first;
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