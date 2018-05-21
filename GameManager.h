#pragma once

#include <memory>
#include <map>
#include "PlayerAlgorithm.h"
#include "Piece.h"
#include "FightInfo.h"
#include "Board.h"
#include "BoardImpl.h"


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
	GameManager(std::shared_ptr<PlayerAlgorithm> algo1, std::shared_ptr<PlayerAlgorithm> algo2);
	int playRound();
private:
	void position(int i, std::vector<std::unique_ptr<FightInfo>>& fights);
	void doMove(int i);
	void changeJoker(int i);
	int output();
	std::unique_ptr<FightInfo> fight(const Point& pos, const std::shared_ptr<Piece> piece1);
	void kill(std::shared_ptr<Piece> piece);
	bool isGameOn();
	std::unique_ptr<Player> _players[2];
	BoardImpl _board;
	unsigned int _numFights;
};