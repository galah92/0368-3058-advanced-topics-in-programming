#pragma once

#include "Point.h"


class PiecePosition {
public:
	virtual const Point& getPosition() const = 0;
	virtual char getPiece() const = 0;
	virtual char getJokerRep() const = 0;
	virtual ~PiecePosition() = default;
};

class PiecePositionImpl : public PiecePosition {
public:
	PiecePositionImpl(const PointImpl& pos, char piece, char jokerRep) :
		_pos(pos),
		_piece(piece),
		_jokerRep(jokerRep) {}
	PiecePositionImpl(int x, int y, char piece, char jokerRep) :
		_pos(PointImpl(x, y)),
		_piece(piece),
		_jokerRep(jokerRep) {}
	const Point& getPosition() const override { return _pos; };
	char getPiece() const override { return _piece; };
	char getJokerRep() const override { return _jokerRep; };
private:
	PointImpl _pos;
	char _piece;
	char _jokerRep;
};