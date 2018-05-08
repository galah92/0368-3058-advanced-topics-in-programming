#include <memory>
#include <vector>
#include "GameManager.h"
#include "PiecePosition.h"
#include "GameContainers.h"

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

bool GameManager::populate(int player, vector<unique_ptr<PiecePosition>> &positions, Fights &fights) {
    GameBoard tmpBoard;
    for (const auto &pos : positions) {
        if (!tmpBoard.getPlayer(pos->getPosition())) return false;
        tmpBoard.setPiece(pos->getPosition(), GamePiece(player, pos->getPiece(), pos->getJokerRep()));
        numFlags[player] += pos->getPiece() == 'J';
        // TODO: increment numMovablePieces if needed
    }
    for (unsigned int i = 0; i < N; i++) {
        for (unsigned int j = 0; j < M; j++) {
            const auto piece = tmpBoard.getPiece(RPSPoint(i, j));
            (void)fights;
        }
    }
    return false;
}

void GameManager::fight(Point &pos, GamePiece &piece, Fights &fights) {
    (void)pos;
    (void)piece;
    (void)fights;
}
