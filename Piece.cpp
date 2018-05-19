#include <vector>
#include <cctype>
#include <algorithm>
#include "Piece.h"


std::shared_ptr<Piece> Piece::Empty = std::make_shared<Piece>(0, PieceType::None, false);

int Piece::getPlayer() const {
	return _player;
}

PieceType Piece::getType() const {
	return _type;
}

bool Piece::setType(PieceType type) {
	if (!_isJoker) return false;
	_type = type;
	return true;
}

bool Piece::isJoker() const {
	return _isJoker;
}

std::unordered_map<PieceType, bool> canMoveMap = {
	{ PieceType::None, false },
	{ PieceType::Flag, false },
	{ PieceType::Rock, true },
	{ PieceType::Paper, true },
	{ PieceType::Scissors, true },
	{ PieceType::Bomb, false },
	{ PieceType::Unknown, false },
};

bool Piece::canMove() const {
	return canMoveMap[_type];
}

std::unordered_map<PieceType, std::vector<PieceType>> canKillMap = {
	{ PieceType::None,{ PieceType::None } },
	{ PieceType::Flag,{ PieceType::None, PieceType::Flag } },
	{ PieceType::Rock,{ PieceType::None, PieceType::Flag, PieceType::Rock, PieceType::Scissors } },
	{ PieceType::Paper,{ PieceType::None, PieceType::Flag, PieceType::Rock, PieceType::Paper } },
	{ PieceType::Scissors,{ PieceType::None, PieceType::Flag, PieceType::Paper, PieceType::Scissors } },
	{ PieceType::Bomb,{ PieceType::None, PieceType::Flag, PieceType::Rock, PieceType::Paper, PieceType::Scissors, PieceType::Bomb } },
	{ PieceType::Unknown,{} },
};

bool Piece::canKill(PieceType piece) const {
    auto const& vec = canKillMap[_type];
    return std::find(vec.begin(), vec.end(), piece) != vec.end();
}

bool Piece::isValid(PieceType piece) {
	return canMoveMap.find(piece) != canMoveMap.end();
}

Piece::operator char() const {
	char ch = _isJoker ? 'J' : (char)_type;
	return _player == 1 ? std::toupper(ch) : std::tolower(ch);
}