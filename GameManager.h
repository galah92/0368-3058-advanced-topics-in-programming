#pragma once

#include <memory>
#include <vector>
#include <unordered_map>
#include "PlayerAlgorithm.h"
#include "Piece.h"


enum class PlayerStatus {
	Playing,
	InvalidPos,
	InvalidMove,
	NoFlags,
	CantMove,
};

class Player {
public:
	Player(int index, std::shared_ptr<PlayerAlgorithm> algo) :
		index(index),
		algo(algo) {}
	std::shared_ptr<PlayerAlgorithm> algo;
	PlayerStatus status = PlayerStatus::Playing;
	unsigned int numFlags = 0;
	unsigned int numMovable = 0;
	int index;
};

class GameManager {
public:
	void play(std::shared_ptr<PlayerAlgorithm> algo1, std::shared_ptr<PlayerAlgorithm> algo2);
private:
	void position(Player& player);
	Player _players[2];
	BoardImpl _board;
};