#pragma once


class GamePiece {
public:
    GamePiece(int player, char piece, char jokerRep = ' ');
    int getPlayer() const;
    char getPiece() const;
    char getJokerRep() const;
    bool setJokerRep(char jokerRep);
    bool isFlag() const;
    bool canMove() const;
    operator char() const;
private:
    int _player;
    char _piece;
    char _jokerRep;
};
