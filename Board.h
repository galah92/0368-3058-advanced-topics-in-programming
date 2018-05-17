#pragma once

#include <memory>
#include <array>
#include "Point.h"
#include "Piece.h"

#define N 10
#define M 10


class Board {
public:
	virtual int getPlayer(const Point& pos) const = 0;
	virtual ~Board() {}
};

class BoardImpl : public Board {
public:
	BoardImpl();
	int getPlayer(const Point& pos) const;
private:
	std::array<std::shared_ptr<Piece>, N * M> _board;
};