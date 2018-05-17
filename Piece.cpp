#pragma once

#include "Piece.h"


Piece Piece::Empty = Piece(0, PieceType::None, false);

int Piece::getPlayer() const {
	return _player;
}

PieceType Piece::getType() const {
	return _type;
}

bool Piece::isJoker() const {
	return _isJoker;
}