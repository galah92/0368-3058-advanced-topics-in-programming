#include "BoardImpl.h"


BoardImpl::BoardImpl() {
    clear();
}

void BoardImpl::clear() {
    _arr.fill(Piece::Empty);
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
    return _arr[pos.getX() *  M + pos.getY()];
}

std::shared_ptr<Piece> BoardImpl::operator[](const Point& pos) const {
    return _arr[pos.getX() *  M + pos.getY()];
}

std::shared_ptr<Piece>& BoardImpl::operator[](const std::pair<int, int> pos) {
    return _arr[pos.first *  M + pos.second];
}

std::shared_ptr<Piece> BoardImpl::operator[](const std::pair<int, int> pos) const {
    return _arr[pos.first *  M + pos.second];
}

std::ostream& operator<<(std::ostream& os, const BoardImpl& board) {
    for (unsigned int i = 0; i < N; i++) {
        for (unsigned int j = 0; j < M; j++) {
            os << *board._arr[i * M + j];
        }
        os << std::endl;
    }
    return os;
}