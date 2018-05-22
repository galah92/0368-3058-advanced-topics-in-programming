#pragma once

#include "Point.h"
#include "Move.h"
#include "JokerChange.h"
#include "FightInfo.h"
#include "PiecePosition.h"


class PointImpl : public Point {
public:
	PointImpl(int x, int y) : _x(x), _y(y) {}
	int getX() const override { return _x; }
	int getY() const override { return _y; }
private:
	int _x;
	int _y;
};

class MoveImpl : public Move {
public:
	MoveImpl(const PointImpl& from, const PointImpl& to) :
		_from(from),
		_to(to) {}
	const Point& getFrom() const override { return _from; };
	const Point& getTo() const override { return _to; };
private:
	const PointImpl _from;
	const PointImpl _to;
};

class JokerChangeImpl : public JokerChange {
public:
	JokerChangeImpl(const PointImpl& pos, char rep) :
		_pos(pos),
		_rep(rep) {}
	const Point& getJokerChangePosition() const override { return _pos; };
	char getJokerNewRep() const override { return _rep; };
private:
	const PointImpl _pos;
	char _rep;
};

class FightInfoImpl : public FightInfo {
public:
	FightInfoImpl(const PointImpl& pos, char piece1, char piece2, int winner) :
		_pos(pos),
		_piece1(piece1),
		_piece2(piece2),
		_winner(winner) {}
	const Point& getPosition() const override { return _pos; };
	char getPiece(int player) const override { return player == 1 ? _piece1 : _piece2; };
	int getWinner() const override { return _winner; };
private:
	const PointImpl _pos;
	char _piece1;
	char _piece2;
	int _winner;
};

class PiecePositionImpl : public PiecePosition {
public:
	PiecePositionImpl(int x, int y, char piece, char jokerRep = ' ') :
		_pos(PointImpl(x, y)),
		_piece(piece),
		_jokerRep(jokerRep) {}
	const Point& getPosition() const override { return _pos; };
	char getPiece() const override { return _piece; };
	char getJokerRep() const override { return _jokerRep; };
private:
	const PointImpl _pos;
	char _piece;
	char _jokerRep;
};