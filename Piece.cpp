#include <vector>
#include <cctype>
#include <algorithm>
#include "Piece.h"


std::shared_ptr<Piece> Piece::Empty = std::make_shared<Piece>(0, PieceType::None, PieceType::None);

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

bool Piece::canKill(PieceType piece) const {
    auto const& vec = canKillMap[_type == PieceType::Joker ? _jokerType : _type];
    return std::find(vec.begin(), vec.end(), piece) != vec.end();
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

Piece::operator char() const {
	return _player == 1 ? std::toupper((char)_type) : std::tolower((char)_type);
}