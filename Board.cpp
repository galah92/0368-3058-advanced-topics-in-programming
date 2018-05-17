#include "Board.h"

BoardImpl::BoardImpl() {
	_board.fill(Piece::Empty);
}

int BoardImpl::getPlayer(const Point& pos) const {
	int x = pos.getX();
	int y = pos.getY();
	if (x < 0 || x >= N || y < 0 || y >= M) return 0;
	return _board[x *  M + y]->getPlayer();
}