#include <cctype>
#include "GameBoard.h" 

using std::isupper;

const char EMPTY = ' ';

int GameBoard::getPlayer(const Point& pos) const {
    unsigned int i = pos.getX() * M + pos.getY();
    if (i >= _board.size() || _board[i] == EMPTY) return 0;
    return isupper(_board[i]) ? 1 : 2;
}
