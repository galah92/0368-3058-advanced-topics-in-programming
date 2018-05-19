#include <memory>
#include "GameManager.h"
#include "FilePlayerAlgorithm.h"


int main()
{
	auto algo1 = std::make_shared<FilePlayerAlgorithm>();
	auto algo2 = std::make_shared<FilePlayerAlgorithm>();
	// GameManager game;
	// game.play(algo1, algo2);
	return 0;
}