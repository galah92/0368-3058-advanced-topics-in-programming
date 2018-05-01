#pragma once

#include <array>
#include <memory>
#include "Board.h"
#include "Point.h"

#define N 10
#define M 10

using std::unique_ptr;
using std::array;


class RPSBoard : public Board {
public:
	int getPlayer(const Point& pos) const;
private:
    array<unique_ptr<char>, N * M> _board;
};
