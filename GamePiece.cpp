#include "GamePiece.h"


GamePiece::GamePiece(int player, char piece, char jokerRep) :
    _player(player), _piece(piece), _jokerRep(jokerRep) { }

int GamePiece::getPlayer() const {
    return _player;
}

bool GamePiece::setJokerRep(char jokerRep) {
    if (_piece != 'J') return false;
    _jokerRep = jokerRep;
    return true;
}
