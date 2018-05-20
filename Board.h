#pragma once

#include <iostream>
#include <memory>
#include <array>
#include "Point.h"
#include "Piece.h"

const int N = 10;
const int M = 10;


class Board {
public:
	virtual int getPlayer(const Point& pos) const = 0;
	virtual ~Board() {}
};

class BoardImpl : public Board {
public:
	BoardImpl();
	void clear();
	int getPlayer(const Point& pos) const override;
	bool isValidPosition(const Point& pos) const;
	friend std::ostream& operator<<(std::ostream& os, const BoardImpl& board);
	std::shared_ptr<Piece>& operator[](const Point& pos);
	std::shared_ptr<Piece> operator[](const Point& pos) const;
private:
	std::array<std::shared_ptr<Piece>, N * M> _board;
};