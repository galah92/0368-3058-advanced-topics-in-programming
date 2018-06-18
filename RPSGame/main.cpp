#include <iostream>
#include <vector>
#include <string>
#include "TournamentManager.h"


int main(int argc, char *argv[]) {
    auto& manager = TournamentManager::getTournamentManager();
    std::vector<std::string> vec(argv + 1, argv + argc);
    vec.push_back(""); // to make is possible to itetate until vec.size() - 1
    for (unsigned int i = 0; i < vec.size() - 1; i++) {
        if (vec[i] == "-threads") {
            manager.maxThreads = std::stoul(vec[i + 1]);
        } else if (vec[i] == "-path") {
            manager.path = vec[i + 1];
        }
    }
    manager.run();
    return 0;
}