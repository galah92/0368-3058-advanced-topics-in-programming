#include "Piece.h"


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