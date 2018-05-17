#include "Board.h"

BoardImpl::BoardImpl() {
	clear();
}

void BoardImpl::clear() {
	_board.fill(Piece::Empty);
}

int BoardImpl::getPlayer(const Point& pos) const {
	auto piece = getPiece(pos);
	return piece != nullptr ? piece->getPlayer() : 0;
}

std::shared_ptr<Piece> BoardImpl::getPiece(const Point & pos) const {
	return isValidPosition(pos) ? _board[pos.getX() *  M + pos.getY()] : nullptr;
}

std::shared_ptr<Piece> BoardImpl::setPiece(const Point & pos, std::shared_ptr<Piece> piece) {
	if (!isValidPosition(pos)) return nullptr;
	_board[pos.getX() *  M + pos.getY()] = piece;
	return piece;
}

bool BoardImpl::isValidPosition(const Point & pos) const {
	int x = pos.getX();
	int y = pos.getY();
	return x >= 0 && x < N && y >= 0 && y < M;
}