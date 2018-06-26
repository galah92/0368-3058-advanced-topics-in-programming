#pragma once

#include <functional>
#include <utility>
#include <memory>
#include <array>
#include <tuple>

template<typename GAME_PIECE>
using PieceInfo = std::unique_ptr<const std::pair<int, GAME_PIECE>>;

template<int ROWS, int COLS, typename GAME_PIECE, int NUM_PLAYERS = 2>
class GameBoard {
public:
    const static int SIZE = ROWS * COLS;
    PieceInfo<GAME_PIECE> getPiece(int row, int col) {
        const auto& pieceInfo = _arr[row * COLS + col];
        if (!pieceInfo) return nullptr;
        return std::make_unique<const std::pair<int, GAME_PIECE>>(pieceInfo->first, pieceInfo->second);
    }
    PieceInfo<GAME_PIECE> setPiece(int row, int col, GAME_PIECE piece, int player) {
        auto pieceInfo = std::make_unique<const std::pair<int, GAME_PIECE>>(player, piece);
        _arr[row * COLS + col].swap(pieceInfo);
        return pieceInfo;
    }
    class iterator {
    public:
        using predicate_type = std::function<bool(const PieceInfo<GAME_PIECE>&)>;
        iterator(const GameBoard& board, const predicate_type& pred, int i = 0) :
            _board(board), _pred(pred), _i(--i) {
            do _i++; while (_i < SIZE && (!_board._arr[_i] || !_pred(_board._arr[_i])));
        }
        iterator& operator++() {
            do _i++; while (_i < SIZE && (!_board._arr[_i] || !_pred(_board._arr[_i])));
            return *this;
        }
        const std::tuple<int, int, GAME_PIECE, int> operator*() {
            const auto& player = _board._arr[_i]->first;
            const auto& piece = _board._arr[_i]->second;
            return std::make_tuple(_i / COLS, _i % COLS, piece, player);
        }
        bool operator==(const iterator& other) { return _i == other._i; }
        bool operator!=(const iterator& other) { return !(*this == other); }
        iterator begin() { return iterator(_board, _pred, 0); }
        iterator end() { return iterator(_board, _pred, SIZE); }
    private:
        const GameBoard& _board;
        const predicate_type _pred;
        int _i;
    };
    iterator begin() { return iterator(*this, [](const auto&) { return true; }, 0); }
    iterator end() { return iterator(*this, [](const auto&) { return true; }, _arr.size()); }
    iterator allPiecesOfPlayer(int playerNum) {
        return iterator(*this, [playerNum](const auto& pieceInfo) {
            return pieceInfo->first == playerNum;
        });
    }
    iterator allOccureneceOfPiece(GAME_PIECE piece) {
        return iterator(*this, [piece](const auto& pieceInfo) {
            return pieceInfo->second == piece;
        });
    }
    iterator allOccureneceOfPieceForPlayer(GAME_PIECE piece, int playerNum) {
        return iterator(*this, [playerNum, piece](const auto& pieceInfo) {
            return pieceInfo->first == playerNum && pieceInfo->second == piece;
        });
    }
private:
    std::array<PieceInfo<GAME_PIECE>, SIZE> _arr;
};