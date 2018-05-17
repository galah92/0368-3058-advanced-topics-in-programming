#pragma once

#include <vector>
#include <memory>
#include "Point.h"
#include "Move.h"
#include "JokerChange.h"
#include "PiecePosition.h"
#include "FightInfo.h"
#include "Board.h"


class PlayerAlgorithm {
public:
	virtual void getInitialPositions(int player, std::vector<std::unique_ptr<PiecePosition>>& vectorToFill) = 0;
	virtual void notifyOnInitialBoard(const Board& b, const std::vector<std::unique_ptr<FightInfo>>& fights) = 0;
	virtual void notifyOnOpponentMove(const Move& move) = 0;
	virtual void notifyFightResult(const FightInfo& fightInfo) = 0;
	virtual std::unique_ptr<Move> getMove() = 0;
	virtual std::unique_ptr<JokerChange> getJokerChange() = 0;
	virtual ~PlayerAlgorithm() {}
};