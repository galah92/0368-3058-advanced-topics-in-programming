#pragma once


class GamePiece {
public:
    GamePiece(int player, char piece, char jokerRep = ' ');
    int getPlayer() const;
    bool setJokerRep(char jokerRep);
private:
    int _player;
    char _piece;
    char _jokerRep;
};
