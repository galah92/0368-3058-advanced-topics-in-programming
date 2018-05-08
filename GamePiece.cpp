#include <locale>
#include <map>
#include "GamePiece.h"

using std::tolower;
using std::toupper;
using std::map;


const static map<char, bool> pieceTypeToCanMove = {
    { ' ', false },
    { 'F', false },
    { 'R', true },
    { 'P', true },
    { 'S', true },
    { 'B', false },
};

GamePiece::GamePiece(int player, char piece, char jokerRep) :
    _player(player), _piece(piece), _jokerRep(jokerRep) { }

int GamePiece::getPlayer() const {
    return _player;
}

char GamePiece::getPiece() const {
    return _piece;
}

char GamePiece::getJokerRep() const {
    return _jokerRep;
}

bool GamePiece::setJokerRep(char jokerRep) {
    if (_piece != 'J') return false;
    _jokerRep = jokerRep;
    return true;
}

bool GamePiece::isFlag() const {
    return _piece == 'F';
}

bool GamePiece::canMove() const {
    return pieceTypeToCanMove.at(_piece == 'J' ? _jokerRep : _piece);
}

GamePiece::operator char() const {
    return _player == 1 ? tolower(_piece) : toupper(_piece);
}
