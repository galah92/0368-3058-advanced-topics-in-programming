#include <cctype>
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


std::shared_ptr<Piece> Piece::Empty = std::make_shared<Piece>(0, PieceType::None, false);

int Piece::getPlayer() const {
	return _player;
}

PieceType Piece::getType() const {
	return _type;
}

bool Piece::isJoker() const {
	return _isJoker;
}

bool Piece::isValid(PieceType piece) {
	return canMove.find(piece) != canMove.end();
}

Piece::operator char() const {
	char ch = _isJoker ? 'J' : (char)_type;
	return _player == 1 ? std::toupper(ch) : std::tolower(ch);
}