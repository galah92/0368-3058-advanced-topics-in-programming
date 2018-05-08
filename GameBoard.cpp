#include "GameBoard.h" 

using std::make_shared;


int GameBoard::getPlayer(const Point &pos) const {
    unsigned int i = pos.getX() * M + pos.getY();
    if (i >= _board.size() || _board[i] == nullptr) return 0;
    return _board[i]->getPlayer();
}

const shared_ptr<GamePiece> GameBoard::getPiece(const Point &pos) const {
    unsigned int i = pos.getX() * M + pos.getY();
    if (i >= _board.size()) return nullptr;
    return _board[i];
}

void GameBoard::setPiece(const Point &pos, const GamePiece &piece) {
    unsigned int i = pos.getX() * M + pos.getY();
    if (i >= _board.size()) return;
    _board[i] = make_shared<GamePiece>(piece);
}
