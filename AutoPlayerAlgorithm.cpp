#include <string>
#include <algorithm>
#include "AutoPlayerAlgorithm.h"
#include "AlgorithmRegistration.h"


// REGISTER_ALGORITHM(203521984);

const auto MOVABLE_PIECES = { 'R', 'P', 'S' };

AutoPlayerAlgorithm::AutoPlayerAlgorithm() : _rg(std::mt19937(std::random_device{}())) {
    _piecesOnBoardCount = { // num of pieces currently hard-coded 
        { 'F', 1 },
        { 'R', 2 },
        { 'P', 5 },
        { 'S', 1 },
        { 'B', 2 },
        { 'J', 2 }
    };
}

void AutoPlayerAlgorithm::getInitialPositions(int player, std::vector<std::unique_ptr<PiecePosition>>& positions) {
    _player = player;
    _board.clear();
    positions.clear();
    initBoard();
    for (auto i = 0; i < N; i++) {
        for (auto j = 0; j < M; j++) {
            const auto& piece = _board[{i, j}];
            if (piece->getPlayer() == player) {
                positions.push_back(std::make_unique<PiecePositionImpl>(i, j, piece->getType(), piece->getJokerType()));
            }
        }
    }
}

void AutoPlayerAlgorithm::notifyOnInitialBoard(const Board& board, const std::vector<std::unique_ptr<FightInfo>>& fights) {
    for (auto y = 0; y < M; y++) {
        for (auto x = 0; x < N; x++) {
            PointImpl pos(x, y);
            if (board.getPlayer(pos) != _player && _board[pos]->getPlayer() == _player) { // player lose a fight
                _board[pos] = std::make_shared<Piece>(1 - _player, 'U'); // Unknown opponent's piece
            }
        }
    }
    for (const auto& fight : fights) notifyFightResult(*fight);
    // TODO: can deduce the number opponent pieces and their type!
}

void AutoPlayerAlgorithm::notifyOnOpponentMove(const Move& move) {
    _board[move.getTo()] = _board[move.getFrom()];
    _board[move.getFrom()] = Piece::Empty;
}

void AutoPlayerAlgorithm::notifyFightResult(const FightInfo& fightInfo) {
    const auto& pos = fightInfo.getPosition();
    const auto ourPiece = fightInfo.getPiece(_player);
    const auto oppPiece = fightInfo.getPiece(1 - _player);
    if (fightInfo.getWinner() == _player) {
        _board[pos] = std::make_shared<Piece>(_player, ourPiece);
    }
    else if (fightInfo.getWinner() == 1 - _player) { // we lost
        _piecesOnBoardCount[ourPiece]--;
        _board[pos] = std::make_shared<Piece>(1 - _player, oppPiece);
    }
    else { // both pieces killed
        _piecesOnBoardCount[ourPiece]--;
        _board[pos] = Piece::Empty;
    }
    // TODO: can deduce the number opponent pieces and their type!
}

std::unique_ptr<Move> AutoPlayerAlgorithm::getMove() {
    std::unique_ptr<PointImpl> from;
    std::unique_ptr<PointImpl> to;
    from = getPosToMoveFrom();
    if (from == nullptr) return nullptr;
    to = getBestNeighbor(from);

    _board[*from] = Piece::Empty; // update board
    if (_board[*to]->getPlayer() != 1 - _player) { // there will be no fight
        _board[*to] = _board[*from];
    }
    return std::make_unique<MoveImpl>(from->getX(), from->getY(), to->getX(), to->getY());
}

std::unique_ptr<JokerChange> AutoPlayerAlgorithm::getJokerChange() {

    for (const auto& pieceType : MOVABLE_PIECES) {
        if (_piecesOnBoardCount[pieceType] > 1) return nullptr; // no need to change Jokers (Jokers are bombs and protect the flag)
    }
    // there are no movable pieces
    for (unsigned int y = 0; y < M; y++) {
        for (unsigned int x = 0; x < N; x++) {
            const auto& piece = _board[{x, y}];
            if (piece->getType() == 'J' && piece->getPlayer() == _player) // reach joker pos
                if (piece->getJokerType() == 'B') { // joker isn't movable rep
                    return std::make_unique<JokerChangeImpl>(PointImpl(x, y), 'S');
                }
        }
    }
    return nullptr;
}

std::unique_ptr<PointImpl> AutoPlayerAlgorithm::getPosToMoveFrom() {
    for (const auto& pieceType : MOVABLE_PIECES) {
        if (_piecesOnBoardCount[pieceType] == 0) continue;
        for (auto y = 0; y < M; y++) {
            for (auto x = 0; x < N; x++) {
                const auto& piece = _board[{x, y}];
                if (piece->getType() != pieceType) continue;
                if (piece->getPlayer() != _player) continue;
                if (hasValidMove(x, y)) return std::make_unique<PointImpl>(x, y);
            }
        }
    }
    // there are no possible moves of movable 
    // handle Joker move
    for (unsigned int y = 0; y < M; y++) {
        for (unsigned int x = 0; x < N; x++) {
            const auto& piece = _board[{x, y}];
            if (piece->getType() != 'J') continue;
            if (std::find(MOVABLE_PIECES.begin(),
                MOVABLE_PIECES.end(),
                piece->getJokerType())
                == MOVABLE_PIECES.end()) continue; // joker rep isn't movable
            if (piece->getPlayer() != _player) continue;
            if (hasValidMove(x, y)) {
                return std::make_unique<PointImpl>(x, y);
            }
        }
    }
    return nullptr;
}

bool AutoPlayerAlgorithm::hasValidMove(int x, int y) {
    auto from = std::make_unique<PointImpl>(x, y);
    auto permutations = validPermutations(from);
    for (const auto& pos : permutations) {
        if (_board[pos]->getPlayer() != _player) return true;
    }
    return false;
}

std::unique_ptr<PointImpl> AutoPlayerAlgorithm::getBestNeighbor(std::unique_ptr<PointImpl>& from) {
    for (const auto& pos : validPermutations(from)) {
        if (_board[pos]->getPlayer() == _player) continue;
        return std::make_unique<PointImpl>(pos);
    }
    return nullptr;
}

std::vector<PointImpl> AutoPlayerAlgorithm::validPermutations(std::unique_ptr<PointImpl>& from) {
    std::vector<PointImpl> vec;
    int x = from->getX();
    int y = from->getY();
    if (_board.isValidPosition(PointImpl(x - 1, y - 1))) vec.push_back(PointImpl(x - 1, y - 1));
    if (_board.isValidPosition(PointImpl(x - 1, y))) vec.push_back(PointImpl(x - 1, y));
    if (_board.isValidPosition(PointImpl(x - 1, y + 1))) vec.push_back(PointImpl(x - 1, y + 1));
    if (_board.isValidPosition(PointImpl(x, y - 1))) vec.push_back(PointImpl(x, y - 1));
    if (_board.isValidPosition(PointImpl(x, y + 1))) vec.push_back(PointImpl(x, y + 1));
    if (_board.isValidPosition(PointImpl(x + 1, y - 1))) vec.push_back(PointImpl(x + 1, y - 1));
    if (_board.isValidPosition(PointImpl(x + 1, y))) vec.push_back(PointImpl(x + 1, y));
    if (_board.isValidPosition(PointImpl(x + 1, y + 1))) vec.push_back(PointImpl(x + 1, y + 1));
    return vec;
}

void AutoPlayerAlgorithm::initBoard() {
    // flag in edge surrounded by bombs & joker
    _board[{0, 0}] = std::make_shared<Piece>(_player, 'F');
    _board[{0, 1}] = std::make_shared<Piece>(_player, 'B');
    _board[{1, 0}] = std::make_shared<Piece>(_player, 'B');
    _board[{1, 1}] = std::make_shared<Piece>(_player, 'J', 'B');
    // currently all other pieces positions are hardcoded
    _board[{5, 5}] = std::make_shared<Piece>(_player, 'J', 'B');
    _board[{2, 7}] = std::make_shared<Piece>(_player, 'R');
    _board[{8, 2}] = std::make_shared<Piece>(_player, 'R');
    _board[{2, 2}] = std::make_shared<Piece>(_player, 'P');
    _board[{3, 7}] = std::make_shared<Piece>(_player, 'P');
    _board[{4, 4}] = std::make_shared<Piece>(_player, 'P');
    _board[{5, 8}] = std::make_shared<Piece>(_player, 'P');
    _board[{7, 4}] = std::make_shared<Piece>(_player, 'P');
    _board[{6, 6}] = std::make_shared<Piece>(_player, 'S');
    // rotate the board by 90deg - flag can be on any edge
    auto n = std::uniform_int_distribution<int>(0, 3)(_rg);
    for (auto i = 0; i < n; i++) rotateBoard();
}

void AutoPlayerAlgorithm::rotateBoard() {
    BoardImpl oldBoard = _board;
    for (auto i = 0; i < N; i++) {
        for (auto j = 0; j < M; j++) {
            _board[{i, j}] = oldBoard[{N - 1 - j, i}];
        }
    }
}