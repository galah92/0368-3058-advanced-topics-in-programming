#include "GameBoard.h" 

using std::make_unique;


int GameBoard::getPlayer(const Point& pos) const {
    unsigned int i = pos.getX() * M + pos.getY();
    if (i >= _board.size() || _board[i] == nullptr) return 0;
    return _board[i]->getPlayer();
}

void GameBoard::setPiece(const PiecePosition &pos, int player) {
    unsigned int i = pos.getPosition().getX() * M + pos.getPosition().getY();
    _board[i] = make_unique<GamePiece>(player, pos.getPiece(), pos.getJokerRep());
}
