// #include <iostream>
// #include <fstream>
#include <vector>
#include "GameManager.h"
#include "Piece.h"
#include "Point.h"
#include "Move.h"
#include "JokerChange.h"
#include "PiecePosition.h"
#include "GameContainers.h"

#define DEBUG(x) do { std::cout << "RSPPlayer203521984::" << __func__ << "()\t" << x << std::endl; } while (0)

int GameManager::playRound(std::shared_ptr<PlayerAlgorithm> algo1, std::shared_ptr<PlayerAlgorithm> algo2) {
    // init
    _players[0] = std::make_unique<Player>(1, algo1);
    _players[1] = std::make_unique<Player>(2, algo2);
    _board.clear();
    _numFights = 0;
    // positioning
    std::vector<std::unique_ptr<FightInfo>> fights;
    position(0, fights);
    position(1, fights);
    if (!isValid(_players[0]) || !isValid(_players[1])) return output();
    _players[0]->algo->notifyOnInitialBoard(_board, fights);
    _players[1]->algo->notifyOnInitialBoard(_board, fights);
    // moves
    auto i = 0;
    while (_numFights < FIGHTS_THRESHOLD) {
        if (!isValid(_players[0]) || !isValid(_players[1])) break;
        doMove(i);
        if (!isValid(_players[0]) || !isValid(_players[1])) break;
        changeJoker(i);
        i = 1 - i; // switch player
    }
    return output();
}

void GameManager::position(int i, std::vector<std::unique_ptr<FightInfo>>& fights) {
    auto& player = _players[i];
    GameBoard<Piece> tmpBoard;
    std::vector<std::unique_ptr<PiecePosition>> positions;
    player->algo->getInitialPositions(player->index, positions);
    // populate tmpBoard & player piece map
    for (const auto& piecePos : positions) {
        if (!isValid(piecePos, tmpBoard)) {
            player->status = PlayerStatus::InvalidPos;
            return;
        }
        const auto& pos = piecePos->getPosition();
        auto type = piecePos->getPiece();
        auto jokerType = piecePos->getJokerRep();
        tmpBoard[pos] = { Piece(player->index, type, jokerType), player->index };
        player->numPieces[type]++;
        if (tmpBoard[pos].piece.getType() == 'F') player->numFlags++;
        if (tmpBoard[pos].piece.canMove()) player->numMovable++;
    }
    if (!isValid(player)) {
        player->status = PlayerStatus::InvalidPos;
        return;
    }
    // merge tmpBoard and main board
    for (unsigned int i = 0; i < GameBoard<Piece>::M; i++) {
        for (unsigned int j = 10; j < GameBoard<Piece>::N; j++) {
            auto fightInfo = fight(PointImpl(i, j), tmpBoard[{i, j}]);
            if (fightInfo) fights.push_back(std::move(fightInfo));
            }
        }
    }

void GameManager::doMove(int i) {
    const auto move = _players[i]->algo->getMove();
    if (!isValid(move, i)) {
        _players[i]->status = PlayerStatus::InvalidMove;
        return;
    }
    _players[1 - i]->algo->notifyOnOpponentMove(*move);
    auto fightInfo = fight(move->getTo(), _board[move->getFrom()]);
    if (fightInfo) {
        _players[0]->algo->notifyFightResult(*fightInfo);
        _players[1]->algo->notifyFightResult(*fightInfo);
        _numFights = 0;
    } else {
        _numFights++;
    }
    _board[move->getFrom()].piece = Piece();
}

void GameManager::changeJoker(int i) {
    auto& player = _players[i];
    const auto jokerChange = player->algo->getJokerChange();
    if (!jokerChange) return;
    if (!isValid(jokerChange, i)) {
        player->status = PlayerStatus::InvalidMove;
        return;
    }
    auto& piece = _board[jokerChange->getJokerChangePosition()].piece;
    piece.setJokerType(jokerChange->getJokerNewRep());
}

int GameManager::output() {
    // std::ofstream fout("rps.output");
    if (_players[0]->status == PlayerStatus::Playing || _players[1]->status == PlayerStatus::Playing) {
        int winner = _players[0]->status == PlayerStatus::Playing ? 0 : 1;
        // int loser = 1 - winner;
        // fout << "Winner: " << winner + 1 << std::endl << "Reason: ";
        // switch (_players[loser]->status) {
        // case PlayerStatus::InvalidPos:
        //     fout << "Bad positioning input for player " << loser + 1 << std::endl;
        //     break;
        // case PlayerStatus::InvalidMove:
        //     fout << "Bad move input for player " << loser + 1 << std::endl;
        //     break;
        // case PlayerStatus::NoFlags:
        //     fout << "All flags of the opponent are captured" << std::endl;
        //     break;
        // case PlayerStatus::CantMove:
        //     fout << "All moving PIECEs of the opponent are eaten" << std::endl;
        //     break;
        // default:
        //     break;
        // }
        // fout << std::endl << _board;
        return winner + 1;
    } else { // tie
        // fout << "Winner: " << 0 << std::endl << "Reason: ";
        // if (_players[0]->status == PlayerStatus::InvalidPos) {
        //     fout << "Bad positioning input for both players" << std::endl;
        // } else { // _players[0].status == PlayerStatus::NoFlags
        //     fout << "Both players cannot play (no flags / cannot move)" << std::endl;
        // }
        // // TODO: handle numFights exceeded
        // fout << std::endl << _board;
        return 0;
    }
}

std::unique_ptr<FightInfo> GameManager::fight(const Point& pos, const GameBoard<Piece>::Entry& entry) {
    if (entry.player == 0 || _board[pos].player == 0) { // no fight for sure
        if (entry.player != 0) {
            _board[pos] = entry; // because _board[pos].player == 0
        } // else no need to change board
        return nullptr;
    } // else fight
    const auto& piece1 = entry.player == 1 ? entry.piece : _board[pos].piece;
    const auto& piece2 = entry.player == 2 ? entry.piece : _board[pos].piece;
    auto killPiece1 = piece2.canKill(piece1);
    auto killPiece2 = piece1.canKill(piece2);
    if (killPiece1) kill(piece1);
    if (killPiece2) kill(piece2);
    const auto winner = (killPiece1 && killPiece2) ? 0 : (killPiece1 ? 2 : 1);
    _board[pos] = { winner == 0 ? Piece() : (winner == 1 ? piece1 : piece2), winner };
    const auto ch1 = piece1.getUnderlyingType();
    const auto ch2 = piece2.getUnderlyingType();
    return std::make_unique<FightInfoImpl>((const PointImpl&)pos, ch1, ch2, winner);
}

void GameManager::kill(const Piece& piece) {
    auto& player = _players[piece.getPlayer() - 1];
    player->numPieces[piece.getType()]--;
    if (piece.getType() == 'F') {
        player->numFlags--;
        if (player->numFlags == 0) player->status = PlayerStatus::NoFlags;
    }
    if (piece.canMove()) {
        player->numMovable--;
        if (player->numMovable == 0) player->status = PlayerStatus::CantMove;
    }
}

bool GameManager::isValid(const std::unique_ptr<Move>& move, int i) const {
    if (!move) return false;
    const auto& from = move->getFrom();
    const auto& to = move->getTo();
    // check that points on board
    if (!_board.isValid(to)) return false;
    if (!_board.isValid(from)) return false;
    // check  that points are next to each other
    auto horizontal = std::abs(from.getX() - to.getX());
    auto vertical = std::abs(from.getY() - to.getY());
    if (horizontal > 1 || vertical > 1) return false;
    if (horizontal == 0 && vertical == 0) return false;
    // check that that piece is the player's piece and that it can move
    if (_board[from].player != i + 1) return false;
    if (!_board[from].piece.canMove()) return false;
    // check that the destination doesn't contain a player's piece
    if (_board[to].player == i + 1) return false;
    return true;
}

bool GameManager::isValid(const std::unique_ptr<JokerChange>& jokerChange, int i) const {
    const auto rep = jokerChange->getJokerNewRep();
    const auto& pos = jokerChange->getJokerChangePosition();
    // check that point on board
    if (!_board.isValid(pos)) return false;
    // check that rep is valid
    if (!Piece::isValid(rep)) return false;
    // check that that piece is the player's piece and that it's a Joker
    if (_board[pos].player != i + 1) return false;
    if (_board[pos].piece.getType() != 'J') return false;
    return true;
}

bool GameManager::isValid(const std::unique_ptr<PiecePosition>& piecePos, const GameBoard<Piece>& board) const {
    if (!piecePos) return false;
    // check that pos is empty
    if (!board.isValid(piecePos->getPosition())) return false;
    if (board[piecePos->getPosition()].player != 0) return false;
    // check that it's a valid piece
    if (!Piece::isValid(piecePos->getPiece(), piecePos->getJokerRep())) return false;
    return true;
}

bool GameManager::isValid(std::unique_ptr<Player>& player) const {
    // check that status is 'playing'
    if (player->status != PlayerStatus::Playing) return false;
    // check that has plags and can move
    if (player->numFlags == 0 || player->numMovable == 0) return false;
    // check didn't exceed pieces capacity
    for (const auto& type : player->numPieces) {
        if (type.second > Piece::maxCapacity.at(type.first)) return false;
    }
    return true;
}