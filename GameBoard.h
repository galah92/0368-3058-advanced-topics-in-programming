#pragma once

#include <array>
#include <memory>
#include "Board.h"
#include "Point.h"
#include "PiecePosition.h"
#include "GamePiece.h"

using std::array;
using std::unique_ptr;

const int N = 10;
const int M = 10;

class GameBoard : public Board {
public:
	int getPlayer(const Point &pos) const;
    void setPiece(const PiecePosition &pos, int player);
private:
    array<unique_ptr<GamePiece>, N * M> _board;
};
