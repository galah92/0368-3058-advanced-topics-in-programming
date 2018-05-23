#pragma once

#include <vector>
#include <memory>
#include "PlayerAlgorithm.h"
#include "PiecePosition.h"
#include "FightInfo.h"
#include "Board.h"
#include "Move.h"
#include "JokerChange.h"
#include "Piece.h"
#include "GameContainers.h"
#include "BoardImpl.h"


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
	std::unique_ptr<PointImpl> getPosToMoveFrom();
	std::unique_ptr<PointImpl> getBestNeighbor(std::unique_ptr<PointImpl>& from);
	std::vector<PointImpl> validPermutations(std::unique_ptr<PointImpl>& from);
	void initBoard();
	void rotateBoard();
	int _player;
	BoardImpl _board;
	std::map<char, unsigned int> _piecesOnBoardCount;
	std::map<char, std::vector<std::shared_ptr<Piece>>> _piecesOnBoard;
	std::array<char, 3> _movablePieces;
};