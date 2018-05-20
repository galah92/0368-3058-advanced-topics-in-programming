#pragma once

#include <vector>
#include <memory>
#include "PlayerAlgorithm.h"
#include "PiecePosition.h"
#include "FightInfo.h"
#include "Board.h"
#include "Move.h"
#include "JokerChange.h"


class AutoPlayerAlgorithm : public PlayerAlgorithm {
public:
	AutoPlayerAlgorithm(){
	_piecesOnBoard = { // num of pieces currently hard-coded 
	{ PieceType::Flag, 1 }, 
	{ PieceType::Rock, 2 },
	{ PieceType::Paper, 5 },
	{ PieceType::Scissors, 1 },
	{ PieceType::Bomb, 2 },
	{ PieceType::Joker, 2 }
	};
}
	void getInitialPositions(int player, std::vector<std::unique_ptr<PiecePosition>>& positions) override;
	void notifyOnInitialBoard(const Board& b, const std::vector<std::unique_ptr<FightInfo>>& fights) override;
	void notifyOnOpponentMove(const Move& move) override;
	void notifyFightResult(const FightInfo& fightInfo) override;
	std::unique_ptr<Move> getMove() override;
	std::unique_ptr<JokerChange> getJokerChange() override;
private:
	std::array<std::shared_ptr<Piece>, N * M> _opponentBoard;
	std::unordered_map<PieceType, unsigned int> _piecesOnBoard;
};