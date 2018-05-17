#pragma once

#include <memory>
#include <unordered_map>


enum class PieceType : char {
	None = ' ',
	Flag = 'F',
	Rock = 'R',
	Paper = 'P',
	Scissors = 'S',
	Bomb = 'B',
	Unknown = '#',
};

class Piece {
public:
	Piece(int player, PieceType type, bool isJoker = false) :
		_player(player),
		_type(type),
		_isJoker(isJoker) {}
	int getPlayer() const;
	PieceType getType() const;
	bool isJoker() const;
	static bool isValid(PieceType piece);
	static std::unordered_map<PieceType, bool> canMove;
	operator char() const;
	static std::shared_ptr<Piece> Empty;
private:
	int _player;
	PieceType _type;
	bool _isJoker;
};