#pragma once

#include <istream>
#include "Point.h"
#include "Move.h"
#include "JokerChange.h"
#include "FightInfo.h"
#include "PiecePosition.h"


class PointImpl : public Point {
public:
    PointImpl(int x, int y) : _x(x), _y(y) {}
    PointImpl(std::istream& is) : _x(is.get()), _y(is.get()) {}
    int getX() const override { return _x; }
    int getY() const override { return _y; }
private:
    int _x;
    int _y;
};

class MoveImpl : public Move {
public:
    MoveImpl(int fromX, int fromY, int toX, int toY) :
        _from(fromX, fromY),
        _to(toX, toY) {}
    MoveImpl(std::istream& is) : _from(is), _to(is) {}
    const Point& getFrom() const override { return _from; };
    const Point& getTo() const override { return _to; };
private:
    PointImpl _from;
    PointImpl _to;
};

class JokerChangeImpl : public JokerChange {
public:
    JokerChangeImpl(const PointImpl& pos, char rep) : _pos(pos), _rep(rep) {}
    JokerChangeImpl(std::istream& is) : _pos(is), _rep(is.get()) {}
    const Point& getJokerChangePosition() const override { return _pos; };
    char getJokerNewRep() const override { return _rep; };
private:
    PointImpl _pos;
    char _rep;
};

class FightInfoImpl : public FightInfo {
public:
    FightInfoImpl(const PointImpl& pos, char piece1, char piece2, int winner) :
        _pos(pos),
        _piece1(piece1),
        _piece2(piece2),
        _winner(winner) {}
    FightInfoImpl(std::istream& is) :
        _pos(is),
        _piece1(is.get()),
        _piece2(is.get()),
        _winner(is.get()) {}
    const Point& getPosition() const override { return _pos; };
    char getPiece(int player) const override { return player == 1 ? _piece1 : _piece2; };
    int getWinner() const override { return _winner; };
private:
    PointImpl _pos;
    char _piece1;
    char _piece2;
    int _winner;
};

class PiecePositionImpl : public PiecePosition {
public:
    PiecePositionImpl(int x, int y, char piece, char jokerRep = ' ') :
        _piece(piece),
        _pos(PointImpl(x, y)),
        _jokerRep(jokerRep) {}
    PiecePositionImpl(std::istream& is) :
        _piece(is.get()),
        _pos(is),
        _jokerRep(is.get()) {}
    const Point& getPosition() const override { return _pos; };
    char getPiece() const override { return _piece; };
    char getJokerRep() const override { return _jokerRep; };
private:
    char _piece;
    PointImpl _pos;
    char _jokerRep;
};