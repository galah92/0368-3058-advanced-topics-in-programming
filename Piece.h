#pragma once

#include <memory>
#include <map>


class Piece {
public:
	Piece(int player, char type, char jokerType = 'J');
	int getPlayer() const;
	char getType() const;
	bool setJokerType(char jokerType);
	bool canMove() const;
	bool canKill(const Piece& piece) const;
	static bool isValid(char type);
	static bool isValid(char type, char jokerType);
	const static std::map<char, unsigned int> maxCapacity;
	static std::shared_ptr<Piece> Empty;
	operator char() const;
private:
	int _player;
	char _type;
	char _jokerType;
};