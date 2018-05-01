#pragma once

#include <vector>
#include <memory>
#include "Point.h"
#include "Move.h"
#include "JokerChange.h"
#include "FightInfo.h"
#include "PiecePosition.h"

using std::unique_ptr;


class RPSPoint : public Point {
public:
    RPSPoint(int x, int y) : _x(x), _y(y) { }
	int getX() const { return _x; };
	int getY() const { return _y; };
private:
    int _x;
    int _y;
};

class RPSMove : public Move {
public:
    RPSMove(Point &from, Point &to) : _from(from), _to(to) { }
	const Point& getFrom() const { return _from; };
	const Point& getTo() const { return _to; };
private:
    Point &_from;
    Point &_to;
};

class RPSJokerChange : public JokerChange {
public:
    RPSJokerChange(Point &pos, char newRep) : _pos(pos), _newRep(newRep) { }
    const Point& getJokerChangePosition() const { return _pos; };
	char getJokerNewRep() const { return _newRep; };
private:
    Point &_pos;
    char _newRep;
};

class RPSFightInfo :  public FightInfo {
public:
    RPSFightInfo(Point &pos, char piece, int winner) :
        _pos(pos),
        _piece(piece),
        _winner(winner) { }
	const Point& getPosition() const { return _pos; };
	char getOpponentPiece() const { return _piece; };
	int getWinner() const { return _winner; };
private:
    Point &_pos;
    char _piece;
    int _winner;
};

class RPSPiecePosition : public PiecePosition {
public:
    RPSPiecePosition(Point &pos, char piece, char jokerRep) : _pos(pos), _piece(piece), _jokerRep(jokerRep) { }
	const Point& getPosition() const {return _pos; };
	char getPiece() const { return _piece; };
	char getJokerRep() const { return _jokerRep; };
private:
    Point &_pos;
    char _piece;
    char _jokerRep;
};