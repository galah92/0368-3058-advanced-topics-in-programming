#include <string>
#include "AutoPlayerAlgorithm.h"
#include "Point.h"


void AutoPlayerAlgorithm::getInitialPositions(int player, std::vector<std::unique_ptr<PiecePosition>>& positions) {
	positions.clear(); // to make sure
					   // random corner?
					   // flag block
	positions.push_back(std::make_unique<PiecePositionImpl>(0, 0, 'F', ' ')); // put the flag in the corner
	positions.push_back(std::make_unique<PiecePositionImpl>(0, 1, 'B', ' ')); // cover the flag 
	positions.push_back(std::make_unique<PiecePositionImpl>(1, 0, 'B', ' ')); // cover the flag
	positions.push_back(std::make_unique<PiecePositionImpl>(1, 1, 'J', 'B')); // cover the flag

																			  // randomize other pieces?
	positions.push_back(std::make_unique<PiecePositionImpl>(5, 5, 'J', 'B'));
	positions.push_back(std::make_unique<PiecePositionImpl>(2, 7, 'R', ' '));
	positions.push_back(std::make_unique<PiecePositionImpl>(8, 2, 'R', ' '));
	positions.push_back(std::make_unique<PiecePositionImpl>(2, 2, 'P', ' '));
	positions.push_back(std::make_unique<PiecePositionImpl>(3, 7, 'P', ' '));
	positions.push_back(std::make_unique<PiecePositionImpl>(4, 4, 'P', ' '));
	positions.push_back(std::make_unique<PiecePositionImpl>(5, 8, 'P', ' '));
	positions.push_back(std::make_unique<PiecePositionImpl>(7, 4, 'P', ' '));
	positions.push_back(std::make_unique<PiecePositionImpl>(6, 6, 'S', ' '));

	(void)player;
}

void AutoPlayerAlgorithm::notifyOnInitialBoard(const Board & b, const std::vector<std::unique_ptr<FightInfo>>& fights) {
	(void)b;
	(void)fights;
}

void AutoPlayerAlgorithm::notifyOnOpponentMove(const Move & move) {
	(void)move;
}

void AutoPlayerAlgorithm::notifyFightResult(const FightInfo & fightInfo) {
	(void)fightInfo;
}

std::unique_ptr<Move> AutoPlayerAlgorithm::getMove() {
	return nullptr;
}

std::unique_ptr<JokerChange> AutoPlayerAlgorithm::getJokerChange() {
	return nullptr;
}