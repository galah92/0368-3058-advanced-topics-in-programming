#pragma once

#include <vector>
#include <deque>
#include <memory>
#include <sstream>
#include <fstream>
#include "PlayerAlgorithm.h"
#include "PiecePosition.h"
#include "JokerChange.h"
#include "FightInfo.h"
#include "Move.h"
#include "Board.h"


class FilePlayerAlgorithm : public PlayerAlgorithm {
public:
    void getInitialPositions(int player, std::vector<std::unique_ptr<PiecePosition>>& positions) override;
    void notifyOnInitialBoard(const Board& b, const std::vector<std::unique_ptr<FightInfo>>& fights) override;
    void notifyOnOpponentMove(const Move& move) override;
    void notifyFightResult(const FightInfo& fightInfo) override;
    std::unique_ptr<Move> getMove() override;
    std::unique_ptr<JokerChange> getJokerChange() override;
private:
    std::deque<std::unique_ptr<Move>> _moves;
    std::deque<std::unique_ptr<JokerChange>> _jokerChanges;
};