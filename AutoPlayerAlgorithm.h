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
	_board.fill(Piece::Empty);
	}
	void getInitialPositions(int player, std::vector<std::unique_ptr<PiecePosition>>& positions) override;
	void notifyOnInitialBoard(const Board& b, const std::vector<std::unique_ptr<FightInfo>>& fights) override;
	void notifyOnOpponentMove(const Move& move) override;
	void notifyFightResult(const FightInfo& fightInfo) override;
	std::unique_ptr<Move> getMove() override;
	std::unique_ptr<JokerChange> getJokerChange() override;
private:
	int _player;
	std::array<std::shared_ptr<Piece>, N * M> _board;
	std::unordered_map<PieceType, unsigned int> _piecesOnBoard;
	void setPiece(int x, int y, std::shared_ptr<Piece> piece);
	void setPiece(const Point& pos, std::shared_ptr<Piece> piece);
	std::shared_ptr<Piece> getPiece(const Point& pos) const;
	std::unique_ptr<PointImpl> getPosToMoveFrom();
	std::unique_ptr<PointImpl> getBestNeighbor(std::unique_ptr<PointImpl>& from);
	bool isValidPosition(const Point& pos) const;
};