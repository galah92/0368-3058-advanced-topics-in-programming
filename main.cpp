#include <iostream>
#include <memory>
#include <string>
#include "GameManager.h"
#include "AutoPlayerAlgorithm.h"
#include "FilePlayerAlgorithm.h"


const std::string DELIMITER = "-vs-";
const std::string USAGE = "Usage: ./ex2 <auto|file>-vs-<auto|file>";

std::shared_ptr<PlayerAlgorithm> initPlayerAlgo(std::string str) {
	if (str == "auto") return std::make_shared<AutoPlayerAlgorithm>();
	if (str == "file") return std::make_shared<FilePlayerAlgorithm>();
	return nullptr;
}

int main(int argc, const char *argv[])
{
	if (argc < 2) {
		std::cout << USAGE << std::endl;
		return -1;
	}
	std::string arg(argv[1]);
	unsigned int pos = arg.find(DELIMITER);
	auto algo1 = initPlayerAlgo(arg.substr(0, pos));
	arg.erase(0, pos + DELIMITER.length());
	auto algo2 = initPlayerAlgo(arg.substr(0, pos));
	if (!algo1 || !algo2) {
		std::cout << USAGE << std::endl;
		return -1;
	}
	GameManager game;
	game.play(algo1, algo2);
	return 0;
}