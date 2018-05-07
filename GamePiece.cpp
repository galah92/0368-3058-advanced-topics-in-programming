#include "GamePiece.h"


GamePiece::GamePiece(int player, char piece, bool isJoker) :
    _player(player), _piece(piece), _isJoker(isJoker) { }

int GamePiece::getPlayer() const {
    return _player;
}

bool GamePiece::setPiece(char piece) {
    if (!_isJoker) return false;
    _piece = piece;
    return true;
}
