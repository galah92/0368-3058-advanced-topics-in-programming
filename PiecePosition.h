#pragma once


class Point;

class PiecePosition {
public:
	virtual const Point& getPosition() const = 0;
	virtual char getPiece() const = 0; // R, P, S, B, J or F
	virtual char getJokerRep() const = 0; // ONLY for Joker: R, P, S or B -- non-Joker may return #
	virtual ~PiecePosition() = default;
};

class PiecePositionImpl : public PiecePosition {
public:
	PiecePositionImpl(Point& pos, char piece, char jokerRep) :
		_pos(pos),
		_piece(piece),
		_jokerRep(jokerRep) {}
	const Point& getPosition() const override { return _pos; };
	char getPiece() const override { return _piece; };
	char getJokerRep() const override { return _jokerRep; };
private:
	Point& _pos;
	char _piece;
	char _jokerRep;
};