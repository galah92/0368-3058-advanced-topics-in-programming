#include "BoardImpl.h"


BoardImpl::BoardImpl() {
	clear();
}

void BoardImpl::clear() {
	_board.fill(Piece::Empty);
}

int BoardImpl::getPlayer(const Point& pos) const {
	if (!isValidPosition(pos)) throw "BoardImpl::getPlayer: Invalid const Point& given";
	return operator[](pos)->getPlayer();
}

bool BoardImpl::isValidPosition(const Point& pos) const {
	int x = pos.getX();
	int y = pos.getY();
	return x >= 0 && x < N && y >= 0 && y < M;
}

std::shared_ptr<Piece>& BoardImpl::operator[](const Point& pos) {
	if (!isValidPosition(pos)) throw "BoardImpl::operator[]: Invalid const Point& given";
	return _board[pos.getX() *  M + pos.getY()];
}

std::shared_ptr<Piece> BoardImpl::operator[](const Point & pos) const {
	// return operator[](pos);
	if (!isValidPosition(pos)) throw "BoardImpl::operator[]: Invalid const Point& given";
	return _board[pos.getX() *  M + pos.getY()];
}

std::ostream & operator<<(std::ostream& os, const BoardImpl& board) {
	for (unsigned int i = 0; i < N; i++) {
		for (unsigned int j = 0; j < M; j++) {
			os << *board._board[i * M + j];
		}
		os << std::endl;
	}
	return os;
}