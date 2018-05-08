#pragma once

#include <vector>
#include <array>
#include <functional>
#include "PlayerAlgorithm.h"
#include "GameBoard.h"

using std::array;
using std::vector;
using std::reference_wrapper;
using Fights = vector<unique_ptr<FightInfo>>;


class GameManager {
public:
    GameManager(PlayerAlgorithm &player1, PlayerAlgorithm &player2);
    void play();
private:
    bool populate(int player, vector<unique_ptr<PiecePosition>> &positions, Fights &fights);
    void fight(Point &pos, GamePiece &piece, Fights &fights);
    array<reference_wrapper<PlayerAlgorithm>, 2> _players;
    array<unsigned int, 2> numFlags;
    array<unsigned int, 2> numMovablePieces;
    GameBoard _board = GameBoard();
};
