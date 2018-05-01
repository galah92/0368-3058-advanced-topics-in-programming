#pragma once

#include <vector>
#include <sstream>
#include <memory>
#include <fstream>
#include "PlayerAlgorithm.h"
#include "PiecePosition.h"

using std::istringstream;
using std::unique_ptr;
using std::vector;
using std::ifstream;


class FilePlayerAlgorithm : public PlayerAlgorithm {
public:
    void getInitialPositions(int player, vector<unique_ptr<PiecePosition>>& positions);
	void notifyOnInitialBoard(const Board& b, const vector<unique_ptr<FightInfo>>& fights);
	void notifyOnOpponentMove(const Move& move);
	void notifyFightResult(const FightInfo& fightInfo);
	unique_ptr<Move> getMove();
	unique_ptr<JokerChange> getJokerChange();
private:
    ifstream _boardstream;
	ifstream _movesstream;
    istringstream _movestream; // store each line in moves file
};
