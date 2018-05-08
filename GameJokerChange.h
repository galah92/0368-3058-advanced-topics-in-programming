#pragma once

#include "JokerChange.h"
#include "Point.h"


class GameJokerChange : public JokerChange {
public:
    GameJokerChange(Point &pos, char rep) : _pos(pos), _rep(rep) { }
    const Point& getJokerChangePosition() const { return _pos; };
    char getJokerNewRep() const { return _rep; };
private:
    Point& _pos;
    char _rep;
};
