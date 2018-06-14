#pragma once

#include <iostream>
#include <array>
#include <utility>
#include "Board.h"
#include "Point.h"


template<class T>
class GameBoard : public Board {
public:
    const static std::size_t N = 10;
    const static std::size_t M = 10;
    using Entry = struct {
        T piece;
        int player;
    };
    void clear();
    int getPlayer(const Point& pos) const override;
    bool isValid(const Point& pos) const;
    Entry& operator[](const Point& pos);
    const Entry& operator[](const Point& pos) const;
    Entry& operator[](const std::pair<int, int> pos);
    const Entry& operator[](const std::pair<int, int> pos) const;
private:
    std::size_t getIndex(const Point& pos);
    const static std::size_t SIZE = N * M;
    std::array<Entry, SIZE> _arr;
};