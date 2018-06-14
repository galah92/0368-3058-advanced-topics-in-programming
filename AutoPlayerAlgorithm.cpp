#include <algorithm>
#include <fstream>
#include "AutoPlayerAlgorithm.h"
#include "AlgorithmRegistration.h"

std::fstream nullstream;
#define DEBUG(x) do { nullstream << "RSPPlayer203521984::" << __func__ << "()\t" << x << std::endl; } while (0)


const auto MOVABLE_PIECES = { 'R', 'P', 'S' };

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
    for (std::size_t i = 0; i < GameBoard<Piece>::M; i++) {
        for (std::size_t j = 0; j < GameBoard<Piece>::N; j++) {
            if (_board[{i, j}].player != player) continue;
            const auto& type = _board[{i, j}].piece.type;
            const auto & jokerType = _board[{i, j}].piece.jokerType;
            DEBUG("adding " << i << " " << j << " " << type << " " << jokerType);
            positions.push_back(std::make_unique<PiecePositionImpl>(i + 1, j + 1, type, jokerType));
        }
    }
    DEBUG("end");
}

void AutoPlayerAlgorithm::notifyOnInitialBoard(const Board& board, const std::vector<std::unique_ptr<FightInfo>>& fights) {
    DEBUG("start");
    for (const auto& fight : fights) notifyFightResult(*fight);
    for (std::size_t i = 0; i < GameBoard<Piece>::M; i++) {
        for (std::size_t j = 0; j < GameBoard<Piece>::N; j++) {
            if (board.getPlayer(PointImpl(i + 1, j + 1)) != _opponent) continue;
            if (_board[{i, j}].player == _opponent) continue;
            _board[{i, j}] = { { 'U', 'U' }, _opponent };
        }
    }
    DEBUG("end");
}

void AutoPlayerAlgorithm::notifyOnOpponentMove(const Move& move) {
    const auto& from = move.getFrom();
    const auto& to = move.getTo();
    if (!_board.isValid(from)) DEBUG("source pos not on board");
    if (!_board.isValid(to)) DEBUG("destination pos not on board");
    if (_board[from].player != _opponent) DEBUG("source pos not of opponent piece");
    if (_board[to].player == _opponent) DEBUG("destination pos of opponent piece");
    _board[to] = _board[from];
    _board[from] = { { ' ', ' ' }, 0 };
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
        _board[pos] = { { oppPiece, ' ' }, _opponent };
    } else if (winner == 0) {
        _numPieces[ourPiece]--;
        _board[pos] = { { ' ', ' ' }, 0 };
    } else DEBUG("invalid winner");
}

std::unique_ptr<Move> AutoPlayerAlgorithm::getMove() {
    DEBUG("here");
    const auto from = getPosToMoveFrom();
    if (from == nullptr) return nullptr;
    const auto to = getBestNeighbor(*from);

    if (_board[*to].player != _opponent) { // there will be no fight
        _board[*to] = _board[*from];
    }
    _board[*from] = { { ' ', ' ' }, 0 }; // update board
    DEBUG(from->getX() + from->getY() + to->getX() + to->getY());
    return std::make_unique<MoveImpl>(from->getX(), from->getY(), to->getX(), to->getY());
}

std::unique_ptr<JokerChange> AutoPlayerAlgorithm::getJokerChange() {
    for (const auto& pieceType : MOVABLE_PIECES) {
        if (_numPieces[pieceType] > 1) {
            DEBUG("returning nullptr");
            return nullptr; // no need to change Jokers (Jokers are bombs and protect the flag)
        }
    }
    // there are no movable pieces
    for (std::size_t i = 0; i < GameBoard<Piece>::M; i++) {
        for (std::size_t j = 0; j < GameBoard<Piece>::N; j++) {
            if (_board[{i, j}].piece.type != 'J') continue;
            if (_board[{i, j}].player != _player) continue;
            if (_board[{i, j}].piece.jokerType != 'B') continue; // it can move
            return std::make_unique<JokerChangeImpl>(PointImpl(i + 1, j + 1), 'S');
        }
    }
    DEBUG("returning nullptr");
    return nullptr;
}

std::unique_ptr<PointImpl> AutoPlayerAlgorithm::getPosToMoveFrom() const {
    for (const auto& pieceType : MOVABLE_PIECES) {
        if (_numPieces.at(pieceType) == 0) continue;
    for (std::size_t i = 0; i < GameBoard<Piece>::M; i++) {
        for (std::size_t j = 0; j < GameBoard<Piece>::N; j++) {
                if (_board[{i, j}].piece.type != pieceType) continue;
                if (_board[{i, j}].piece.jokerType != _player) continue;
                if (hasValidMove(i, j)) return std::make_unique<PointImpl>(i + 1, j + 1);
            }
        }
    }
    // there are no possible moves of movable 
    // handle Joker move
    for (std::size_t i = 0; i < GameBoard<Piece>::M; i++) {
        for (std::size_t j = 0; j < GameBoard<Piece>::N; j++) {
            if (_board[{i, j}].piece.type != 'J') continue;
            if (std::find(MOVABLE_PIECES.begin(),
                MOVABLE_PIECES.end(),
                _board[{i, j}].piece.jokerType)
                == MOVABLE_PIECES.end()) continue; // joker isn't movable
            if (_board[{i, j}].player != _player) continue;
            if (hasValidMove(i, j)) {
                return std::make_unique<PointImpl>(i + 1, j + 1);
            }
        }
    }
    DEBUG("returning nullptr");
    return nullptr;
}

bool AutoPlayerAlgorithm::hasValidMove(int x, int y) const {
    for (const auto& pos : validPermutations(PointImpl(x, y))) {
        if (_board[pos].player != _player) return true;
    }
    DEBUG("returning false");
    return false;
}

std::unique_ptr<PointImpl> AutoPlayerAlgorithm::getBestNeighbor(const Point& from) const {
    for (const auto& pos : validPermutations(from)) {
        if (_board[pos].player == _player) continue;
        return std::make_unique<PointImpl>(pos);
    }
    DEBUG("returning nullptr");
    return nullptr;
}

std::vector<PointImpl> AutoPlayerAlgorithm::validPermutations(const Point& from) const {
    std::vector<PointImpl> vec;
    auto x = from.getX();
    auto y = from.getY();
    if (_board.isValid(PointImpl(x - 1, y - 1))) vec.push_back(PointImpl(x - 1, y - 1));
    if (_board.isValid(PointImpl(x - 1, y))) vec.push_back(PointImpl(x - 1, y));
    if (_board.isValid(PointImpl(x - 1, y + 1))) vec.push_back(PointImpl(x - 1, y + 1));
    if (_board.isValid(PointImpl(x, y - 1))) vec.push_back(PointImpl(x, y - 1));
    if (_board.isValid(PointImpl(x, y + 1))) vec.push_back(PointImpl(x, y + 1));
    if (_board.isValid(PointImpl(x + 1, y - 1))) vec.push_back(PointImpl(x + 1, y - 1));
    if (_board.isValid(PointImpl(x + 1, y))) vec.push_back(PointImpl(x + 1, y));
    if (_board.isValid(PointImpl(x + 1, y + 1))) vec.push_back(PointImpl(x + 1, y + 1));
    return vec;
}

void AutoPlayerAlgorithm::initBoard() {
    // flag in edge surrounded by bombs & joker
    _board[{1, 1}] = { { 'F', 'B' }, _player };
    _board[{1, 2}] = { { 'B', 'B' }, _player };
    _board[{2, 1}] = { { 'B', 'B' }, _player };
    _board[{2, 2}] = { { 'J', 'B' }, _player };
    // currently all other pieces positions are hardcoded
    _board[{6, 6}] = { { 'J', 'B' }, _player };
    _board[{3, 8}] = { { 'R', 'B' }, _player };
    _board[{9, 3}] = { { 'R', 'B' }, _player };
    _board[{3, 3}] = { { 'P', 'B' }, _player };
    _board[{4, 8}] = { { 'P', 'B' }, _player };
    _board[{5, 5}] = { { 'P', 'B' }, _player };
    _board[{6, 9}] = { { 'P', 'B' }, _player };
    _board[{7, 5}] = { { 'P', 'B' }, _player };
    _board[{8, 7}] = { { 'S', 'B' }, _player };
    // rotate the board by 90deg - flag can be on any edge
    auto n = std::uniform_int_distribution<std::size_t>(0, 3)(_rg);
    for (std::size_t i = 0; i < n; i++) rotateBoard();
}

void AutoPlayerAlgorithm::rotateBoard() {
    GameBoard<Piece> oldBoard = _board;
    for (std::size_t i = 0; i < GameBoard<Piece>::N; i++) {
        for (std::size_t j = 0; j < GameBoard<Piece>::M; j++) {
            _board[{i, j}] = oldBoard[{GameBoard<Piece>::N - 1 - j, i}];
        }
    }
}

REGISTER_ALGORITHM(203521984)