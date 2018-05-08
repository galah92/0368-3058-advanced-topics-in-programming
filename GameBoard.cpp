#include "GameBoard.h" 


int GameBoard::getPlayer(const Point &pos) const {
    unsigned int i = pos.getX() * M + pos.getY();
    if (i >= _board.size() || _board[i] == nullptr) return 0;
    return _board[i]->getPlayer();
}

bool GameBoard::isValidPosition(const Point& pos) const {
    if (pos.getX() < 0 || pos.getX() >= N) return false;
    if (pos.getY() < 0 || pos.getY() >= M) return false;
    return true;
}

const shared_ptr<GamePiece> GameBoard::getPiece(const Point &pos) const {
    unsigned int i = pos.getX() * M + pos.getY();
    if (i >= _board.size()) return nullptr;
    return _board[i];
}

void GameBoard::setPiece(const Point &pos, shared_ptr<GamePiece> piece) {
    unsigned int i = pos.getX() * M + pos.getY();
    if (i >= _board.size()) return;
    _board[i] = piece;
}
