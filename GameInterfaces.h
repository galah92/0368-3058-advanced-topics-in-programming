#pragma once

#include <vector>
#include <memory>

using std::unique_ptr;


class Point {
public:
	virtual int getX() const = 0;
	virtual int getY() const = 0;
	virtual ~Point() {}
};

class Board {
public:
	virtual int getPlayer(const Point& pos) const = 0;
	virtual ~Board() {}
};

class Move {
public:
	virtual const Point& getFrom() const = 0;
	virtual const Point& getTo() const = 0;
	virtual ~Move() {}
};

class JokerChange {
public:
	virtual const Point& getJokerChangePosition() const = 0;
	virtual char getJokerNewRep() const = 0;
	virtual ~JokerChange() {}
};

class FightInfo {
public:
	virtual const Point& getPosition() const = 0;
	virtual char getOpponentPiece() const = 0;
	virtual int getWinner() const = 0;
	virtual ~FightInfo() {}
};

class PiecePosition {
public:
	virtual const Point& getPosition() const = 0;
	virtual char getPiece() const = 0;
	virtual char getJokerRep() const = 0;
	virtual ~PiecePosition() {}
};

class PlayerAlgorithm {
public:
	virtual void getInitialPositions(int player, std::vector<unique_ptr<PiecePosition>>& vectorToFill) = 0;
	virtual void notifyOnInitialBoard(const Board& b, const std::vector<unique_ptr<FightInfo>>& fights) = 0;
	virtual void notifyOnOpponentMove(const Move& move) = 0;
	virtual void notifyFightResult(const FightInfo& fightInfo) = 0;
	virtual unique_ptr<Move> getMove() = 0;
	virtual unique_ptr<JokerChange> getJokerChange() = 0;
	virtual ~PlayerAlgorithm() {}
};
