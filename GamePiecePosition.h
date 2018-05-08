#pragma once

#include "PiecePosition.h"
#include "Point.h"


class GamePiecePosition : public PiecePosition {
public:
    GamePiecePosition(Point& pos, char piece, char jokerRep) :
        _pos(pos), _piece(piece), _jokerRep(jokerRep) { }
    const Point& getPosition() const { return _pos; };
    char getPiece() const { return _piece; };
    char getJokerRep() const { return _jokerRep; };
private:
    Point& _pos;
    char _piece;
    char _jokerRep;
};
