#pragma once

#include <map>


class Piece {
public:
    Piece(int player = 0, char type = ' ', char jokerType = ' ');
    int getPlayer() const;
    char getUnderlyingType() const;
    char getType() const;
    char getJokerType() const;
    bool setJokerType(char jokerType);
    bool canMove() const;
    bool canKill(const Piece& piece) const;
    static bool isValid(char type);
    static bool isValid(char type, char jokerType);
    const static std::map<char, unsigned int> maxCapacity;
    operator char() const;
private:
    int _player;
    char _type;
    char _jokerType;
};