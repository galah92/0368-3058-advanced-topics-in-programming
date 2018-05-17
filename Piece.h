#pragma once


enum class PieceType {
	None,
	Flag,
	Rock,
	Paper,
	Scissors,
	Bomb,
	Unknown,
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
	static Piece Empty;
private:
	int _player;
	PieceType _type;
	bool _isJoker;
};