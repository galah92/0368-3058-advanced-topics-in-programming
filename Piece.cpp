#include <vector>
#include <cctype>
#include <limits>
#include <algorithm>
#include "Piece.h"


const unsigned int F = 1;
const unsigned int R = 2;
const unsigned int P = 5;
const unsigned int S = 1;
const unsigned int B = 2;
const unsigned int J = 2;

std::shared_ptr<Piece> Piece::Empty = std::make_shared<Piece>(0, PieceType::None, PieceType::None);

Piece::Piece(int player, PieceType type, PieceType jokerType) :
	_player(player),
	_type(type),
	_jokerType(jokerType) {}

int Piece::getPlayer() const {
	return _player;
}

PieceType Piece::getType() const {
	return _type;
}

bool Piece::setJokerType(PieceType jokerType) {
	if (_type != PieceType::Joker) return false;
	_jokerType = jokerType;
	return true;
}

std::unordered_map<PieceType, bool> canMoveMap = {
	{ PieceType::None, false },
	{ PieceType::Flag, false },
	{ PieceType::Rock, true },
	{ PieceType::Paper, true },
	{ PieceType::Scissors, true },
	{ PieceType::Bomb, false },
};

bool Piece::canMove() const {
	const auto& type = _type == PieceType::Joker ? _jokerType : _type;
	return canMoveMap[type];
}

std::unordered_map<PieceType, std::vector<PieceType>> canKillMap = {
	{ PieceType::None, { PieceType::None } },
	{ PieceType::Flag, { PieceType::None, PieceType::Flag } },
	{ PieceType::Rock, { PieceType::None, PieceType::Flag, PieceType::Rock, PieceType::Scissors, PieceType::Bomb } },
	{ PieceType::Paper, { PieceType::None, PieceType::Flag, PieceType::Rock, PieceType::Paper, PieceType::Bomb } },
	{ PieceType::Scissors, { PieceType::None, PieceType::Flag, PieceType::Paper, PieceType::Scissors, PieceType::Bomb } },
	{ PieceType::Bomb, { PieceType::None, PieceType::Flag, PieceType::Rock, PieceType::Paper, PieceType::Scissors, PieceType::Bomb } },
};

bool Piece::canKill(const Piece& piece) const {
	const auto& type = piece._type == PieceType::Joker ? piece._jokerType : piece._type;
	const auto& vec = canKillMap[_type == PieceType::Joker ? _jokerType : _type];
	return std::find(vec.begin(), vec.end(), type) != vec.end();
}

bool Piece::isValid(PieceType type) {
	switch (type) {
	case PieceType::Flag:
	case PieceType::Rock:
	case PieceType::Paper:
	case PieceType::Scissors:
	case PieceType::Bomb:
		return true;
	case PieceType::None:
	case PieceType::Joker:
	default:
		return false;
	}
	return false;
}

bool Piece::isValid(PieceType type, PieceType jokerType) {
	if (type == PieceType::Joker) return canMoveMap.find(jokerType) != canMoveMap.end();
	return type == PieceType::Joker ? isValid(jokerType) : isValid(type);
}

std::unordered_map<PieceType, unsigned int> maxCapacityMap{
	{ PieceType::None, std::numeric_limits<unsigned int>::max() },
	{ PieceType::Flag, F },
	{ PieceType::Rock, R },
	{ PieceType::Paper, P },
	{ PieceType::Scissors, S },
	{ PieceType::Bomb, B },
	{ PieceType::Joker, J },
};

unsigned int Piece::maxCapacity(PieceType type) {
	return maxCapacityMap[type];
}

Piece::operator char() const {
	return _player == 1 ? std::toupper((char)_type) : std::tolower((char)_type);
}