#pragma once

#include <memory>
#include <vector>
#include <map>
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
	Player() = default;
	Player(int index, std::shared_ptr<PlayerAlgorithm> algo) :
		algo(algo),
		status(PlayerStatus::Playing),
		numFlags(0),
		numMovable(0),
		index(index) {}
	std::shared_ptr<PlayerAlgorithm> algo;
	PlayerStatus status = PlayerStatus::Playing;
	std::map<PieceType, unsigned int> numPieces;
	unsigned int numFlags;
	unsigned int numMovable;
	int index;
};

class GameManager {
public:
	void play(std::shared_ptr<PlayerAlgorithm> algo1, std::shared_ptr<PlayerAlgorithm> algo2);
private:
	void position(Player& player);
	void doMove(Player& player);
	void changeJoker(Player& player);
	void output();
	std::shared_ptr<Piece> fight(std::shared_ptr<Piece> piece1, std::shared_ptr<Piece> piece2);
	bool isGameOn();
	Player _players[2];
	BoardImpl _board;
	unsigned int _numFights;
};