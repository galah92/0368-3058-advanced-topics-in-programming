#pragma once

#include "Move.h"
#include "Point.h"


class GameMove : public Move {
public:
    GameMove(const Point& from, const Point& to) : _from(from), _to(to) { }
    const Point& getFrom() const { return _from; };
    const Point& getTo() const { return _to; };
private:
    const Point& _from;
    const Point& _to;
};
