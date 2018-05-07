#pragma once


class GamePiece {
public:
    GamePiece(int player, char piece, bool isJoker);
    int getPlayer() const;
    bool setPiece(char piece);
private:
    int _player;
    char _piece;
    bool _isJoker;
};
