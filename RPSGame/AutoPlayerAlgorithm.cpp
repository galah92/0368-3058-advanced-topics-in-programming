#include <algorithm>
#include <fstream>
#include <iomanip>
#include <cctype>
#include <set>
#include "AutoPlayerAlgorithm.h"
#include "AlgorithmRegistration.h"

std::fstream nullstream;
#define DEBUG(x) do { nullstream << "RSPPlayer203521984::" << __func__ << "\t\t" << x << std::endl; } while (0)


const std::set<char> MOVABLE_PIECES = { 'R', 'P', 'S' };

AutoPlayerAlgorithm::AutoPlayerAlgorithm() : _rg(std::mt19937(std::random_device{}())) {
    _numPieces = {
        { 'F', 1 },
        { 'R', 2 },
        { 'P', 5 },
        { 'S', 1 },
        { 'B', 2 },
        { 'J', 2 },
    };
}

void AutoPlayerAlgorithm::getInitialPositions(int player, std::vector<std::unique_ptr<PiecePosition>>& positions) {
    _player = player;
    _opponent = _player == 1 ? 2 : 1;
    _board.clear();
    positions.clear();
    initBoard();
    for (auto i = 0; i < _board.N; i++) {
        for (auto j = 0; j < _board.M; j++) {
            if (_board[{i, j}].player != player) continue;
            const auto& type = _board[{i, j}].piece.type;
            const auto & jokerType = _board[{i, j}].piece.jokerType;
            positions.push_back(std::make_unique<PiecePositionImpl>(i + 1, j + 1, type, jokerType));
        }
    }
}

void AutoPlayerAlgorithm::notifyOnInitialBoard(const Board& board, const std::vector<std::unique_ptr<FightInfo>>& fights) {
    for (const auto& fight : fights) notifyFightResult(*fight);
    for (auto y = 1; y <= _board.M; y++) {
        for (auto x = 1; x <= _board.N; x++) {
            GamePoint pos(x, y);
            if (board.getPlayer(pos) != _opponent) continue;
            if (_board[pos].player == _opponent) continue;
            _board[pos] = { { 'u', 'u' }, _opponent };
        }
    }
}

void AutoPlayerAlgorithm::notifyOnOpponentMove(const Move& move) {
    const auto& from = move.getFrom();
    const auto& to = move.getTo();
    if (!_board.isValid(from)) DEBUG("source pos not on board");
    if (!_board.isValid(to)) DEBUG("destination pos not on board");
    if (_board[from].player != _opponent) DEBUG("source pos not of opponent piece");
    if (_board[to].player == _opponent) DEBUG("destination pos of opponent piece");
    _board[to] = _board[from];
    _board[from] = { Piece(), 0 };
}

void AutoPlayerAlgorithm::notifyFightResult(const FightInfo& fightInfo) {
    const auto& pos = fightInfo.getPosition();
    if (!_board.isValid(pos)) DEBUG("pos not on board");
    if (_board[pos].player == 0) DEBUG("pos is empty");
    const auto ourPiece = fightInfo.getPiece(_player);
    const auto oppPiece = fightInfo.getPiece(_opponent);
    const auto winner = fightInfo.getWinner();
    if (winner == _player) {
        _board[pos] = { { ourPiece, ' ' }, _player };
    } else if (winner == _opponent) {
        _numPieces[ourPiece]--;
        _board[pos] = { { (char)std::tolower(oppPiece), ' ' }, _opponent };
    } else if (winner == 0) {
        _numPieces[ourPiece]--;
        _board[pos] = { Piece(), 0 };
    } else DEBUG("invalid winner");
}

std::unique_ptr<Move> AutoPlayerAlgorithm::getMove() {
    // DEBUG(std::endl << _board);
    const auto from = getPosToMoveFrom();
    if (from == nullptr) return nullptr;
    const auto to = getBestNeighbor(*from);
    if (_board[*to].player != _opponent) { // there will be no fight
        _board[*to] = _board[*from];
    }
    _board[*from] = { Piece(), 0 }; // update board
    return std::make_unique<GameMove>(from->getX(), from->getY(), to->getX(), to->getY());
}

std::unique_ptr<JokerChange> AutoPlayerAlgorithm::getJokerChange() {
    for (const auto& pieceType : MOVABLE_PIECES) {
        if (_numPieces[pieceType] > 1) return nullptr; // no need to change jokers
    }
    // there are no movable pieces
    for (auto y = 0; y < _board.M; y++) {
        for (auto x = 0; x < _board.N; x++) {
            if (_board[{x, y}].piece.type != 'J') continue;
            if (_board[{x, y}].player != _player) continue;
            if (_board[{x, y}].piece.jokerType != 'B') continue; // it can move
            return std::make_unique<GameJokerChange>(GamePoint(x + 1, y + 1), 'S');
        }
    }
    return nullptr;
}

std::unique_ptr<GamePoint> AutoPlayerAlgorithm::getPosToMoveFrom() const {
    for (auto y = 0; y < _board.M; y++) {
        for (auto x = 0; x < _board.N; x++) {
            if (_board[{x, y}].player != _player) continue;
            if (!isMovable(_board[{x, y}].piece)) continue;
            if (hasValidMove(x + 1, y + 1)) {
                return std::make_unique<GamePoint>(x + 1, y + 1);
            }
        }
    }
    return nullptr;
}

bool AutoPlayerAlgorithm::hasValidMove(int x, int y) const {
    for (const auto& pos : validPermutations(GamePoint(x, y))) {
        if (_board[pos].player != _player) return true;
    }
    return false;
}

bool AutoPlayerAlgorithm::isMovable(const Piece& piece) const {
    const auto& type = piece.type;
    const auto& jokerType = piece.jokerType;
    return MOVABLE_PIECES.count(type) || (type == 'J' && MOVABLE_PIECES.count(jokerType));
}

std::unique_ptr<GamePoint> AutoPlayerAlgorithm::getBestNeighbor(const Point& from) const {
    for (const auto& pos : validPermutations(from)) {
        if (_board[pos].player == _player) continue;
        return std::make_unique<GamePoint>(pos);
    }
    return nullptr;
}

std::vector<GamePoint> AutoPlayerAlgorithm::validPermutations(const Point& from) const {
    std::vector<GamePoint> vec;
    auto x = from.getX();
    auto y = from.getY();
    if (_board.isValid(GamePoint(x - 1, y - 1))) vec.emplace_back(x - 1, y - 1);
    if (_board.isValid(GamePoint(x - 1, y))) vec.emplace_back(x - 1, y);
    if (_board.isValid(GamePoint(x - 1, y + 1))) vec.emplace_back(x - 1, y + 1);
    if (_board.isValid(GamePoint(x, y - 1))) vec.emplace_back(x, y - 1);
    if (_board.isValid(GamePoint(x, y + 1))) vec.emplace_back(x, y + 1);
    if (_board.isValid(GamePoint(x + 1, y - 1))) vec.emplace_back(x + 1, y - 1);
    if (_board.isValid(GamePoint(x + 1, y))) vec.emplace_back(x + 1, y);
    if (_board.isValid(GamePoint(x + 1, y + 1))) vec.emplace_back(x + 1, y + 1);
    return vec;
}

void AutoPlayerAlgorithm::initBoard() {
    // flag in edge surrounded by bombs & joker
    _board[{0, 0}] = { { 'F', 'B' }, _player };
    _board[{0, 1}] = { { 'B', 'B' }, _player };
    _board[{1, 0}] = { { 'B', 'B' }, _player };
    _board[{1, 1}] = { { 'J', 'B' }, _player };
    // currently all other pieces positions are hardcoded
    _board[{1, 2}] = { { 'J', 'B' }, _player };
    _board[{2, 2}] = { { 'R', 'B' }, _player };
    _board[{2, 3}] = { { 'R', 'B' }, _player };
    _board[{9, 9}] = { { 'P', 'B' }, _player };
    _board[{2, 0}] = { { 'P', 'B' }, _player };
    _board[{9, 0}] = { { 'P', 'B' }, _player };
    _board[{1, 3}] = { { 'P', 'B' }, _player };
    _board[{0, 9}] = { { 'P', 'B' }, _player };
    _board[{0, 2}] = { { 'S', 'B' }, _player };
    // rotate the board by 90deg - flag can be on any edge
    auto n = std::uniform_int_distribution<int>(0, 3)(_rg);
    for (auto i = 0; i < n; i++) rotateBoard();
}

void AutoPlayerAlgorithm::rotateBoard() {
    GameBoard<Piece> oldBoard = _board;
    for (auto i = 0; i < _board.N; i++) {
        for (auto j = 0; j < _board.M; j++) {
            _board[{i, j}] = oldBoard[{_board.N - 1 - j, i}];
        }
    }
}

REGISTER_ALGORITHM(203521984)