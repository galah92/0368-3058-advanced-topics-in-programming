#include <string>
#include "AutoPlayerAlgorithm.h"
#include "Point.h"

// AutoPlayerAlgorithm::AutoPlayerAlgorithm(){
	
// 	_piecesOnBoard = { // num of pieces currently hard-coded 
// 	{ PieceType::Flag, 1 }, 
// 	{ PieceType::Rock, 2 },
// 	{ PieceType::Paper, 5 },
// 	{ PieceType::Scissors, 1 },
// 	{ PieceType::Bomb, 2 },
// 	{ PieceType::Joker, 2 }
// 	};
// }

void AutoPlayerAlgorithm::getInitialPositions(int player, std::vector<std::unique_ptr<PiecePosition>>& positions) {
	positions.clear(); // to make sure
					   // random corner?
					   // flag block
	
	positions.push_back(std::make_unique<PiecePositionImpl>(0, 0, 'F', ' ')); // put the flag in the corner
	setPiece(0, 0, std::make_shared<Piece>(player, (PieceType)'F', PieceType::Joker));
	positions.push_back(std::make_unique<PiecePositionImpl>(0, 1, 'B', ' ')); // cover the flag 
	setPiece(0, 1, std::make_shared<Piece>(player, (PieceType)'B', PieceType::Joker));	
	positions.push_back(std::make_unique<PiecePositionImpl>(1, 0, 'B', ' ')); // cover the flag
	setPiece(1, 0, std::make_shared<Piece>(player, (PieceType)'B', PieceType::Joker));
	positions.push_back(std::make_unique<PiecePositionImpl>(1, 1, 'J', 'B')); // cover the flag
	setPiece(1, 1, std::make_shared<Piece>(player, PieceType::Joker, (PieceType)'B'));

																			  // randomize other pieces?
	positions.push_back(std::make_unique<PiecePositionImpl>(5, 5, 'J', 'B'));
	setPiece(5, 5, std::make_shared<Piece>(player, PieceType::Joker, (PieceType)'B'));
	positions.push_back(std::make_unique<PiecePositionImpl>(2, 7, 'R', ' '));
	setPiece(2, 7, std::make_shared<Piece>(player, (PieceType)'R', PieceType::Joker));
	positions.push_back(std::make_unique<PiecePositionImpl>(8, 2, 'R', ' '));
	setPiece(8, 2, std::make_shared<Piece>(player, (PieceType)'R', PieceType::Joker));
	positions.push_back(std::make_unique<PiecePositionImpl>(2, 2, 'P', ' '));
	setPiece(2, 2, std::make_shared<Piece>(player, (PieceType)'P', PieceType::Joker));
	positions.push_back(std::make_unique<PiecePositionImpl>(3, 7, 'P', ' '));
	setPiece(3, 7, std::make_shared<Piece>(player, (PieceType)'P', PieceType::Joker));
	positions.push_back(std::make_unique<PiecePositionImpl>(4, 4, 'P', ' '));
	setPiece(4, 4, std::make_shared<Piece>(player, (PieceType)'P', PieceType::Joker));
	positions.push_back(std::make_unique<PiecePositionImpl>(5, 8, 'P', ' '));
	setPiece(5, 8, std::make_shared<Piece>(player, (PieceType)'P', PieceType::Joker));
	positions.push_back(std::make_unique<PiecePositionImpl>(7, 4, 'P', ' '));
	setPiece(7, 4, std::make_shared<Piece>(player, (PieceType)'P', PieceType::Joker));
	positions.push_back(std::make_unique<PiecePositionImpl>(6, 6, 'S', ' '));
	setPiece(6, 6, std::make_shared<Piece>(player, (PieceType)'S', PieceType::Joker));

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