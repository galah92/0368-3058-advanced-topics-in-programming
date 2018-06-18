#include <fstream>
#include <vector>
#include "GameManager.h"
#include "Piece.h"
#include "Point.h"
#include "Move.h"
#include "JokerChange.h"
#include "PiecePosition.h"

std::fstream nullstream;
#define DEBUG(x) do { nullstream << "GameManager::" << __func__ << "()\t" << x << std::endl; } while (0)

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
    for (unsigned int i = 1; i <= _board.N; i++) {
        for (unsigned int j = 1; j <= _board.N; j++) {
            GamePoint pos(i, j);
            auto fightInfo = fight(pos, tmpBoard[pos].piece);
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
    auto fightInfo = fight(move->getTo(), _board[move->getFrom()].piece);
    if (fightInfo) {
        _players[0]->algo->notifyFightResult(*fightInfo);
        _players[1]->algo->notifyFightResult(*fightInfo);
        _numFights = 0;
    } else {
        _numFights++;
    }
    _board[move->getFrom()] = { Piece(), 0 };
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
    auto is1Playing = _players[0]->status == PlayerStatus::Playing;
    auto is2Playing = _players[1]->status == PlayerStatus::Playing;
    return (is1Playing == is2Playing) ? 0 : (is1Playing ? 1 : 2);
}

std::unique_ptr<FightInfo> GameManager::fight(const Point& pos, const Piece& piece1) {
    auto piece2 = _board[pos].piece;
    auto killPiece1 = piece2.canKill(piece1);
    auto killPiece2 = piece1.canKill(piece2);
    if (killPiece1 && piece1.getPlayer() != 0) kill(piece1);
    if (killPiece2 && piece2.getPlayer() != 0) kill(piece2);
    auto piece = killPiece1 && killPiece2 ? Piece() : (killPiece1 ? piece2 : piece1);
    _board[pos] = { piece, piece.getPlayer() };
    if (piece1.getPlayer() == 0 || piece2.getPlayer() == 0) return nullptr;
    auto winner = (killPiece1 && killPiece2) ? 0 : (killPiece1 ? piece2.getPlayer() : piece1.getPlayer());
    auto ch1 = (piece1.getPlayer() == 1 ? piece1 : piece2).getUnderlyingType();
    auto ch2 = (piece1.getPlayer() == 2 ? piece1 : piece2).getUnderlyingType();
    return std::make_unique<GameFightInfo>((const GamePoint&)pos, ch1, ch2, winner);
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