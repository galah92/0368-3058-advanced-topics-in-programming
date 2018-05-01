#include <memory>
#include <vector>
#include "GameManager.h"
#include "PiecePosition.h"

using std::unique_ptr;
using std::vector;


RPSGameManager::RPSGameManager(PlayerAlgorithm &player1, PlayerAlgorithm &player2) :
    _player1(player1),
    _player2(player2) { }
    // _board(RPSBoard()) { }

void RPSGameManager::play() {
    vector<unique_ptr<PiecePosition>> positions;
    _player1.getInitialPositions(1, positions);
    // TODO: populate board
    _player2.getInitialPositions(2, positions);
    // TODO: populate board
}
