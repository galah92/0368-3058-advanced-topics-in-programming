#pragma once

#include "PlayerAlgorithm.h"
#include "Board.h"


class RPSGameManager {
public:
    RPSGameManager(PlayerAlgorithm &player1, PlayerAlgorithm &player2);
    void play();
private:
    PlayerAlgorithm &_player1;
    PlayerAlgorithm &_player2;
    // Board &_board;
};
