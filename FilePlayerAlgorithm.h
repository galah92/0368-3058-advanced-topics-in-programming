#pragma once

#include <vector>
#include <memory>
#include <sstream>
#include <fstream>
#include "PlayerAlgorithm.h"
#include "PiecePosition.h"
#include "FightInfo.h"
#include "Board.h"
#include "Move.h"
#include "JokerChange.h"


class FilePlayerAlgorithm : public PlayerAlgorithm {
public:
    void getInitialPositions(int player, std::vector<std::unique_ptr<PiecePosition>>& positions);
	void notifyOnInitialBoard(const Board& b, const std::vector<std::unique_ptr<FightInfo>>& fights);
	void notifyOnOpponentMove(const Move& move);
	void notifyFightResult(const FightInfo& fightInfo);
	std::unique_ptr<Move> getMove();
	std::unique_ptr<JokerChange> getJokerChange();
private:
    std::ifstream _boardstream;
	std::ifstream _movesstream;
    std::istringstream _movestream; // store each line in moves file
};
