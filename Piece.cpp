#include <vector>
#include <cctype>
#include <algorithm>
#include "Piece.h"


std::unordered_map<PieceType, bool> Piece::canMove = {
	{ PieceType::None, false },
	{ PieceType::Flag, false },
	{ PieceType::Rock, true },
	{ PieceType::Paper, true },
	{ PieceType::Scissors, true },
	{ PieceType::Bomb, false },
	{ PieceType::Unknown, false },
};

std::unordered_map<PieceType, std::vector<PieceType>> mapCanKill = {
	{ PieceType::None, { PieceType::None } },
	{ PieceType::Flag, { PieceType::None, PieceType::Flag } },
	{ PieceType::Rock, { PieceType::None, PieceType::Flag, PieceType::Rock, PieceType::Scissors } },
	{ PieceType::Paper, { PieceType::None, PieceType::Flag, PieceType::Rock, PieceType::Paper } },
	{ PieceType::Scissors, { PieceType::None, PieceType::Flag, PieceType::Paper, PieceType::Scissors } },
	{ PieceType::Bomb, { PieceType::None, PieceType::Flag, PieceType::Rock, PieceType::Paper, PieceType::Scissors, PieceType::Bomb } },
	{ PieceType::Unknown, { } },
};

std::shared_ptr<Piece> Piece::Empty = std::make_shared<Piece>(0, PieceType::None, false);

int Piece::getPlayer() const {
	return _player;
}

PieceType Piece::getType() const {
	return _type;
}

bool Piece::canKill(PieceType piece) {
    auto const& vec = mapCanKill[_type];
    return std::find(vec.begin(), vec.end(), piece) != vec.end();
}

bool Piece::isValid(PieceType piece) {
	return canMove.find(piece) != canMove.end();
}

Piece::operator char() const {
	char ch = _isJoker ? 'J' : (char)_type;
	return _player == 1 ? std::toupper(ch) : std::tolower(ch);
}
