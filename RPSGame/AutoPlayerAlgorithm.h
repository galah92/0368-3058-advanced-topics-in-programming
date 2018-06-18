#pragma once

#include <random>
#include <memory>
#include <vector>
#include <map>
#include "PlayerAlgorithm.h"
#include "GameContainers.h"
#include "PiecePosition.h"
#include "JokerChange.h"
#include "FightInfo.h"
#include "Board.h"
#include "Move.h"


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
    struct Piece {
        char type = ' ';
        char jokerType = ' ';
        friend std::ostream& operator<<(std::ostream& os, const Piece piece) { return os << piece.type; }
    };
    std::unique_ptr<GamePoint> getPosToMoveFrom() const;
    std::unique_ptr<GamePoint> getBestNeighbor(const Point& from) const;
    std::vector<GamePoint> validPermutations(const Point& from) const;
    bool hasValidMove(int x, int y) const;
    bool isMovable(const Piece& piece) const;
    void initBoard();
    void rotateBoard();
    int _player;
    int _opponent;
    GameBoard<Piece> _board;
    std::mt19937 _rg;
    std::map<char, unsigned int> _numPieces;
};

using RSPPlayer_203521984 = AutoPlayerAlgorithm;