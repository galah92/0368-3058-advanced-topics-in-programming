#pragma once

#include <vector>
#include <array>
#include <functional>
#include "PlayerAlgorithm.h"
#include "PiecePosition.h"
#include "GameBoard.h"


class GameManager {
public:
    GameManager(PlayerAlgorithm &player1, PlayerAlgorithm &player2);
    void play();
private:
    bool populate(int player, std::vector<std::unique_ptr<PiecePosition>> &positions, std::vector<std::unique_ptr<FightInfo>> &fights);
    bool isValidMove(std::unique_ptr<Move>& move, int player);
    bool isValidJokerChange(std::unique_ptr<JokerChange>& change, int player);
	std::shared_ptr<GamePiece> fight(std::shared_ptr<GamePiece> piece1, std::shared_ptr<GamePiece> piece2);
	std::array<std::reference_wrapper<PlayerAlgorithm>, 2> _players;
	std::array<unsigned int, 2> numFlags;
	std::array<unsigned int, 2> numMovablePieces;
    GameBoard _board = GameBoard();
};