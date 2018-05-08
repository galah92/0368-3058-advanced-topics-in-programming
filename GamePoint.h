#pragma once

#include "Point.h"

class GamePoint : public Point {
public:
    GamePoint(int x, int y) : _x(x), _y(y) { }
    int getX() const { return _x; };
    int getY() const { return _y; };
private:
    int _x;
    int _y;
};
