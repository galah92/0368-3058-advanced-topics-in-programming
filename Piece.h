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
	Joker = 'J',
};

class Piece {
public:
	Piece(int player, PieceType type, PieceType jokerType);
	int getPlayer() const;
	PieceType getType() const;
	bool setJokerType(PieceType jokerType);
	bool canMove() const;
	bool canKill(const Piece& piece) const;
	static bool isValid(PieceType type);
	static bool isValid(PieceType type, PieceType jokerType);
	static bool maxCapacity(PieceType type);
	static std::shared_ptr<Piece> Empty;
	operator char() const;
private:
	int _player;
	PieceType _type;
	PieceType _jokerType;
};