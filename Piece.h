#pragma once

#include <memory>
#include <map>


enum class PieceType : char {
	None = ' ',
	Flag = 'F',
	Rock = 'R',
	Paper = 'P',
	Scissors = 'S',
	Bomb = 'B',
	Joker = 'J',
	Unknown = 'U',
};

class Piece {
public:
	Piece(int player, PieceType type, PieceType jokerType = PieceType::None);
	Piece(int player, char type, char jokerType = 'J');
	int getPlayer() const;
	PieceType getType() const;
	bool setJokerType(PieceType jokerType);
	bool canMove() const;
	bool canKill(const Piece& piece) const;
	static bool isValid(PieceType type);
	static bool isValid(PieceType type, PieceType jokerType);
	static unsigned int maxCapacity(PieceType type);
	static std::shared_ptr<Piece> Empty;
	operator char() const;
private:
	int _player;
	PieceType _type;
	PieceType _jokerType;
};