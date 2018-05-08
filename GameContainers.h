#pragma once

#include <vector>
#include <memory>
#include "Point.h"
#include "Move.h"
#include "JokerChange.h"
#include "FightInfo.h"
#include "PiecePosition.h"

using std::unique_ptr;


class GameFightInfo :  public FightInfo {
public:
    GameFightInfo(Point &pos, char piece1, char piece2, int winner) :
        _pos(pos),
        _piece1(piece1),
        _piece2(piece2),
        _winner(winner) { }
	const Point& getPosition() const { return _pos; };
	char getPiece(int player) const { return player == 1 ? _piece1 : _piece2; };
	int getWinner() const { return _winner; };
private:
    Point &_pos;
    char _piece1;
    char _piece2;
    int _winner;
};
