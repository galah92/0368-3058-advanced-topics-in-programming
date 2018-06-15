#include <vector>
#include <limits>
#include <algorithm>
#include "Piece.h"


Piece::Piece(int player, char type, char jokerType) :
    _player(player),
    _type(type),
    _jokerType(jokerType) {}

char Piece::getUnderlyingType() const {
    if (_type == 'J') return _jokerType;
    return _type;
}

char Piece::getType() const {
    return _type;
}

char Piece::getJokerType() const {
    return _jokerType;
}

bool Piece::setJokerType(char jokerType) {
    if (_type != 'J') return false;
    _jokerType = jokerType;
    return true;
}

std::map<char, bool> canMoveMap = {
    { ' ', false },
    { 'F', false },
    { 'R', true },
    { 'P', true },
    { 'S', true },
    { 'B', false },
};

bool Piece::canMove() const {
    const auto& type = _type == 'J' ? _jokerType : _type;
    return canMoveMap[type];
}

std::map<char, std::vector<char>> canKillMap = {
    { ' ', { ' ' } },
    { 'F', { ' ', 'F' } },
    { 'R', { ' ', 'F', 'R', 'S', 'B' } },
    { 'P', { ' ', 'F', 'R', 'P', 'B' } },
    { 'S', { ' ', 'F', 'P', 'S', 'B' } },
    { 'B', { ' ', 'F', 'R', 'P', 'S', 'B' } },
};

bool Piece::canKill(const Piece& piece) const {
    const auto& type = piece._type == 'J' ? piece._jokerType : piece._type;
    const auto& vec = canKillMap[_type == 'J' ? _jokerType : _type];
    return std::find(vec.begin(), vec.end(), type) != vec.end();
}

bool Piece::isValid(char type) {
    switch (type) {
    case 'F':
    case 'R':
    case 'P':
    case 'S':
    case 'B':
        return true;
    default:
        return false;
    }
}

bool Piece::isValid(char type, char jokerType) {
    if (type == 'J') return canMoveMap.find(jokerType) != canMoveMap.end();
    return type == 'J' ? isValid(jokerType) : isValid(type);
}

const std::map<char, unsigned int> Piece::maxCapacity = {
    { ' ', std::numeric_limits<unsigned int>::max() },
    { 'F', 1 },
    { 'R', 2 },
    { 'P', 5 },
    { 'S', 1 },
    { 'B', 2 },
    { 'J', 2 },
};

Piece::operator char() const {
    return _player == 1 ? std::toupper(_type) : std::tolower(_type);
}