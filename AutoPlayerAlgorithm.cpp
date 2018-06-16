#include <algorithm>
#include <fstream>
#include "AutoPlayerAlgorithm.h"
#include "AlgorithmRegistration.h"

std::fstream nullstream;
#define DEBUG(x) do { std::cout << "RSPPlayer203521984::" << __func__ << "()\t" << x << std::endl; } while (0)


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
    for (auto i = 1; i <= N; i++) {
        for (auto j = 1; j <= M; j++) {
            const auto& piece = _board[{i, j}];
            if (piece->getPlayer() != player) continue;
            const auto& type = piece->getType();
            const auto & jokerType = piece->getJokerType();
            positions.push_back(std::make_unique<PiecePositionImpl>(i, j, type, jokerType));
        }
    }
}

void AutoPlayerAlgorithm::notifyOnInitialBoard(const Board& board, const std::vector<std::unique_ptr<FightInfo>>& fights) {
    for (const auto& fight : fights) notifyFightResult(*fight);
    for (auto y = 1; y <= M; y++) {
        for (auto x = 1; x <= N; x++) {
            GamePoint pos(x, y);
            if (board.getPlayer(pos) != _opponent) continue;
            if (_board[pos]->getPlayer() == _opponent) continue;
            _board[pos] = std::make_shared<Piece>(_opponent, 'U');
        }
    }
}

void AutoPlayerAlgorithm::notifyOnOpponentMove(const Move& move) {
    const auto& from = move.getFrom();
    const auto& to = move.getTo();
    if (!_board.isValidPosition(from)) DEBUG("source pos not on board");
    if (!_board.isValidPosition(to)) DEBUG("destination pos not on board");
    if (_board[from]->getPlayer() != _opponent) DEBUG("source pos not of opponent piece");
    if (_board[to]->getPlayer() == _opponent) DEBUG("destination pos of opponent piece");
    _board[to] = _board[from];
    _board[from] = Piece::Empty;
}

void AutoPlayerAlgorithm::notifyFightResult(const FightInfo& fightInfo) {
    // DEBUG(std::string("ch1: ") + fightInfo.getPiece(1) + std::string(", ch2: ") + fightInfo.getPiece(2) + std::string(" winner:") + std::to_string(fightInfo.getWinner()));
    const auto& pos = fightInfo.getPosition();
    if (!_board.isValidPosition(pos)) DEBUG("pos not on board");
    if (_board[pos]->getPlayer() == 0) DEBUG("pos is empty");
    const auto ourPiece = fightInfo.getPiece(_player);
    const auto oppPiece = fightInfo.getPiece(_opponent);
    const auto winner = fightInfo.getWinner();
    if (winner == _player) {
        _board[pos] = std::make_shared<Piece>(_player, ourPiece);
    } else if (winner == _opponent) {
        _numPieces[ourPiece]--;
        _board[pos] = std::make_shared<Piece>(_opponent, oppPiece);
    } else if (winner == 0) {
        _numPieces[ourPiece]--;
        _board[pos] = Piece::Empty;
    } else DEBUG("invalid winner");
}

std::unique_ptr<Move> AutoPlayerAlgorithm::getMove() {
    const auto from = getPosToMoveFrom();
    if (from == nullptr) return nullptr;
    const auto to = getBestNeighbor(*from);

    if (_board[*to]->getPlayer() != _opponent) { // there will be no fight
        _board[*to] = _board[*from];
    }
    _board[*from] = Piece::Empty; // update board
    return std::make_unique<GameMove>(from->getX(), from->getY(), to->getX(), to->getY());
}

std::unique_ptr<JokerChange> AutoPlayerAlgorithm::getJokerChange() {
    for (const auto& pieceType : MOVABLE_PIECES) {
        if (_numPieces[pieceType] > 1) return nullptr; // no need to change Jokers (Jokers are bombs and protect the flag)
    }
    // there are no movable pieces
    for (auto y = 1; y <= M; y++) {
        for (auto x = 1; x <= N; x++) {
            if (_board[{x, y}]->getType() != 'J') continue;
            if (_board[{x, y}]->getPlayer() != _player) continue;
            if (_board[{x, y}]->getJokerType() != 'B') continue; // it can move
            return std::make_unique<GameJokerChange>(GamePoint(x, y), 'S');
        }
    }
    return nullptr;
}

std::unique_ptr<GamePoint> AutoPlayerAlgorithm::getPosToMoveFrom() const {
    for (const auto& pieceType : MOVABLE_PIECES) {
        if (_numPieces.at(pieceType) == 0) continue;
        for (auto y = 1; y <= M; y++) {
            for (auto x = 1; x <= N; x++) {
                const auto& piece = _board[{x, y}];
                if (piece->getType() != pieceType) continue;
                if (piece->getPlayer() != _player) continue;
                if (hasValidMove(x, y)) return std::make_unique<GamePoint>(x, y);
            }
        }
    }
    // there are no possible moves of movable 
    // handle Joker move
    for (auto y = 1; y <= M; y++) {
        for (auto x = 1; x <= N; x++) {
            if (_board[{x, y}]->getType() != 'J') continue;
            if (std::find(MOVABLE_PIECES.begin(),
                MOVABLE_PIECES.end(),
                _board[{x, y}]->getJokerType())
                == MOVABLE_PIECES.end()) continue; // joker isn't movable
            if (_board[{x, y}]->getPlayer() != _player) continue;
            if (hasValidMove(x, y)) {
                return std::make_unique<GamePoint>(x, y);
            }
        }
    }
    return nullptr;
}

bool AutoPlayerAlgorithm::hasValidMove(int x, int y) const {
    for (const auto& pos : validPermutations(GamePoint(x, y))) {
        if (_board[pos]->getPlayer() != _player) return true;
    }
    return false;
}

std::unique_ptr<GamePoint> AutoPlayerAlgorithm::getBestNeighbor(const Point& from) const {
    for (const auto& pos : validPermutations(from)) {
        if (_board[pos]->getPlayer() == _player) continue;
        return std::make_unique<GamePoint>(pos);
    }
    return nullptr;
}

std::vector<GamePoint> AutoPlayerAlgorithm::validPermutations(const Point& from) const {
    std::vector<GamePoint> vec;
    auto x = from.getX();
    auto y = from.getY();
    if (_board.isValidPosition(GamePoint(x - 1, y - 1))) vec.push_back(GamePoint(x - 1, y - 1));
    if (_board.isValidPosition(GamePoint(x - 1, y))) vec.push_back(GamePoint(x - 1, y));
    if (_board.isValidPosition(GamePoint(x - 1, y + 1))) vec.push_back(GamePoint(x - 1, y + 1));
    if (_board.isValidPosition(GamePoint(x, y - 1))) vec.push_back(GamePoint(x, y - 1));
    if (_board.isValidPosition(GamePoint(x, y + 1))) vec.push_back(GamePoint(x, y + 1));
    if (_board.isValidPosition(GamePoint(x + 1, y - 1))) vec.push_back(GamePoint(x + 1, y - 1));
    if (_board.isValidPosition(GamePoint(x + 1, y))) vec.push_back(GamePoint(x + 1, y));
    if (_board.isValidPosition(GamePoint(x + 1, y + 1))) vec.push_back(GamePoint(x + 1, y + 1));
    return vec;
}

void AutoPlayerAlgorithm::initBoard() {
    // flag in edge surrounded by bombs & joker
    _board[{1, 1}] = std::make_shared<Piece>(_player, 'F');
    _board[{1, 2}] = std::make_shared<Piece>(_player, 'B');
    _board[{2, 1}] = std::make_shared<Piece>(_player, 'B');
    _board[{2, 2}] = std::make_shared<Piece>(_player, 'J', 'B');
    // currently all other pieces positions are hardcoded
    _board[{6, 6}] = std::make_shared<Piece>(_player, 'J', 'B');
    _board[{3, 8}] = std::make_shared<Piece>(_player, 'R');
    _board[{9, 3}] = std::make_shared<Piece>(_player, 'R');
    _board[{3, 3}] = std::make_shared<Piece>(_player, 'P');
    _board[{4, 8}] = std::make_shared<Piece>(_player, 'P');
    _board[{5, 5}] = std::make_shared<Piece>(_player, 'P');
    _board[{6, 9}] = std::make_shared<Piece>(_player, 'P');
    _board[{7, 5}] = std::make_shared<Piece>(_player, 'P');
    _board[{8, 7}] = std::make_shared<Piece>(_player, 'S');
    // rotate the board by 90deg - flag can be on any edge
    auto n = std::uniform_int_distribution<int>(0, 3)(_rg);
    for (auto i = 0; i < n; i++) rotateBoard();
}

void AutoPlayerAlgorithm::rotateBoard() {
    BoardImpl oldBoard = _board;
    for (auto i = 1; i <= N; i++) {
        for (auto j = 1; j <= M; j++) {
            _board[{i, j}] = oldBoard[{N + 1 - j, i}];
        }
    }
}

REGISTER_ALGORITHM(203521984)