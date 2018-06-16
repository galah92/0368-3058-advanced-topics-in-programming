#pragma once

#include <iostream>
#include <utility>
#include <array>
#include "Point.h"
#include "PiecePosition.h"
#include "Move.h"
#include "JokerChange.h"
#include "FightInfo.h"
#include "Board.h"


class GamePoint : public Point {
public:
    GamePoint(int x, int y) : _x(x), _y(y) {}
    GamePoint(std::istream& is) : _x(is.get()), _y(is.get()) {}
    int getX() const override { return _x; }
    int getY() const override { return _y; }
private:
    int _x;
    int _y;
};

class PiecePositionImpl : public PiecePosition {
public:
    PiecePositionImpl(int x, int y, char piece, char jokerRep = ' ') :
        _piece(piece),
        _pos(GamePoint(x, y)),
        _jokerRep(jokerRep) {}
    PiecePositionImpl(std::istream& is) :
        _piece(is.get()),
        _pos(is),
        _jokerRep(is.get()) {}
    const Point& getPosition() const override { return _pos; };
    char getPiece() const override { return _piece; };
    char getJokerRep() const override { return _jokerRep; };
private:
    char _piece;
    GamePoint _pos;
    char _jokerRep;
};

class GameMove : public Move {
public:
    GameMove(int fromX, int fromY, int toX, int toY) :
        _from(fromX, fromY),
        _to(toX, toY) {}
    GameMove(std::istream& is) : _from(is), _to(is) {}
    const Point& getFrom() const override { return _from; };
    const Point& getTo() const override { return _to; };
private:
    GamePoint _from;
    GamePoint _to;
};

class GameJokerChange : public JokerChange {
public:
    GameJokerChange(const GamePoint& pos, char rep) : _pos(pos), _rep(rep) {}
    GameJokerChange(std::istream& is) : _pos(is), _rep(is.get()) {}
    const Point& getJokerChangePosition() const override { return _pos; };
    char getJokerNewRep() const override { return _rep; };
private:
    GamePoint _pos;
    char _rep;
};

class GameFightInfo : public FightInfo {
public:
    GameFightInfo(const GamePoint& pos, char piece1, char piece2, int winner) :
        _pos(pos),
        _piece1(piece1),
        _piece2(piece2),
        _winner(winner) {}
    GameFightInfo(std::istream& is) :
        _pos(is),
        _piece1(is.get()),
        _piece2(is.get()),
        _winner(is.get()) {}
    const Point& getPosition() const override { return _pos; };
    char getPiece(int player) const override { return player == 1 ? _piece1 : _piece2; };
    int getWinner() const override { return _winner; };
private:
    GamePoint _pos;
    char _piece1;
    char _piece2;
    int _winner;
};

template<class T>
class GameBoard : public Board {
public:
    const static std::size_t N = 10;
    const static std::size_t M = 10;
    using Entry = struct { T piece; int player; };
    GameBoard() { clear(); }
    void clear() { _arr.fill({ T(), 0 }); }
    int getPlayer(const Point& pos) const override { return _arr[getIndex(pos)].player; }
    bool isValid(const Point& pos) const { return getIndex(pos) >= 0 && getIndex(pos) < SIZE; }
    Entry& operator[](const Point& pos) { return _arr[getIndex(pos)]; }
    const Entry& operator[](const Point& pos) const { return _arr[getIndex(pos)]; }
    Entry& operator[](const std::pair<int, int>& pos) { return _arr[getIndex(pos)]; }
    const Entry& operator[](const std::pair<int, int>& pos) const { return _arr[getIndex(pos)]; }
    friend std::ostream& operator<<(std::ostream& os, const GameBoard<T> board) {
        for (std::size_t i = 0; i < board.N; i++) {
            for (std::size_t j = 0; j < board.M; j++) {
                os << board[{i, j}].piece;
            }
            os << std::endl;
        }
        return os;
    }
private:
    std::size_t getIndex(const Point& pos) const { return (pos.getX() - 1) * M + (pos.getY() - 1); }
    std::size_t getIndex(const std::pair<int, int>& pos) const { return pos.first * M + pos.second; }
    const static std::size_t SIZE = N * M;
    std::array<Entry, SIZE> _arr;
};