#pragma once

#include <random>
#include <memory>
#include <vector>
#include <map>
#include "PlayerAlgorithm.h"
#include "GameContainers.h"
#include "PiecePosition.h"
#include "JokerChange.h"
#include "BoardImpl.h"
#include "FightInfo.h"
#include "Board.h"
#include "Move.h"
#include "Piece.h"


class AutoPlayerAlgorithm : public PlayerAlgorithm {
public:
    AutoPlayerAlgorithm();
    void getInitialPositions(int player, std::vector<std::unique_ptr<PiecePosition>>& positions) override;
    void notifyOnInitialBoard(const Board& b, const std::vector<std::unique_ptr<FightInfo>>& fights) override;
    void notifyOnOpponentMove(const Move& move) override;
    void notifyFightResult(const FightInfo& fightInfo) override;
    std::unique_ptr<Move> getMove() override;
    std::unique_ptr<JokerChange> getJokerChange() override;
private:
    std::unique_ptr<PointImpl> getPosToMoveFrom() const;
    std::unique_ptr<PointImpl> getBestNeighbor(const Point& from) const;
    std::vector<PointImpl> validPermutations(const Point& from) const;
    bool hasValidMove(int x, int y) const;
    void initBoard();
    void rotateBoard();
    int _player;
    int _opponent;
    BoardImpl _board;
    std::mt19937 _rg;
    std::map<char, unsigned int> _numPieces;
};

using RSPPlayer_203521984 = AutoPlayerAlgorithm;