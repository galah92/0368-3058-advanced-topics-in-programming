#pragma once

#include <array>
#include "Board.h"
#include "Point.h"

using std::array;

const int N = 10;
const int M = 10;

class GameBoard : public Board {
public:
	int getPlayer(const Point& pos) const;
private:
    array<char, N * M> _board;
};
