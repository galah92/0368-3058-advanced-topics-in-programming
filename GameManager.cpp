#include <memory>
#include <vector>
#include "GameManager.h"
#include "PiecePosition.h"

using std::unique_ptr;
using std::vector;
using std::ref;


GameManager::GameManager(PlayerAlgorithm &player1, PlayerAlgorithm &player2) :
    _players({ ref(player1), ref(player2) }) { }

void GameManager::play() {
    vector<unique_ptr<PiecePosition>> positions;
    vector<unique_ptr<FightInfo>> fights;
    _players[0].get().getInitialPositions(1, positions);
    // bool result = populate(1, positions);
    // TODO: handle populate fail
    _players[1].get().getInitialPositions(2, positions);
    // result = populate(2, positions);
    // TODO: handle populate fail
}

bool GameManager::populate(int player, vector<unique_ptr<PiecePosition>> &positions, vector<unique_ptr<FightInfo>> &fights) {
    GameBoard tmpBoard;
    for (const auto &pos : positions) {
        if (!tmpBoard.getPlayer(pos->getPosition())) return false;
        tmpBoard.setPiece(*pos, player);
        numFlags[player] += pos->getPiece() == 'J';
        // TODO: increment numMovablePieces if needed
    }
    return false;
}
