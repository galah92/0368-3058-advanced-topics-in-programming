#include <iostream>
#include <memory>
#include <string>
#include <map>
#include <vector>
#include "TournamentManager.h"
#include "GameManager.h"
#include "AutoPlayerAlgorithm.h"
#include "FilePlayerAlgorithm.h"


const std::string DELIMITER = "-vs-";
const std::string USAGE = "Usage: ./ex2 <auto|file>-vs-<auto|file>";

std::shared_ptr<PlayerAlgorithm> PlayerAlgorithmFactory(std::string str) {
    if (str == "auto") return std::make_shared<AutoPlayerAlgorithm>();
    if (str == "file") return std::make_shared<FilePlayerAlgorithm>();
    return nullptr;
}

std::map<std::string, std::string> parseArgs(int argc, const char *argv[]) {
    std::vector<std::string> vec(argv + 1, argv + argc);
    std::map<std::string, std::string> args;
    for (unsigned int i = 0; i < vec.size(); i += 2) {
        args[vec[i]] = vec[i + 1];
    }
    return args;
}

const std::string DEFAULT_PATH = "./";
const unsigned int DEFAULT_THREADS = 4;

int main(int argc, const char *argv[])
{
    auto args = parseArgs(argc, argv);
    auto& manager = TournamentManager::get();
    manager.path = args.find("-path") == args.end() ? DEFAULT_PATH : args["-path"];
    manager.maxThreads = args.find("-threads") == args.end() ? DEFAULT_THREADS : std::stoi(args["-threads"]);
    std::cout << manager.path << " " << manager.maxThreads << std::endl;
    bool result = manager.run();
    if (!result) {
        std::cout << "Error: no shared libraries loaded" << std::endl;
        return -1;
    }

    if (argc < 2) {
        std::cout << USAGE << std::endl;
        return -1;
    }
    std::string arg(argv[1]);
    auto pos = arg.find(DELIMITER);
    auto algo1 = PlayerAlgorithmFactory(arg.substr(0, pos));
    arg.erase(0, pos + DELIMITER.length());
    auto algo2 = PlayerAlgorithmFactory(arg.substr(0, pos));
    if (!algo1 || !algo2) {
        std::cout << USAGE << std::endl;
        return -1;
    }
    GameManager game;
    game.playRound(algo1, algo2);
    return 0;
}