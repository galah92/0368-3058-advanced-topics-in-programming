#pragma once

#include <memory>
#include <map>
#include "GameContainers.h"
#include "PlayerAlgorithm.h"
#include "Piece.h"
#include "FightInfo.h"
#include "Board.h"


class GameManager {
public:
    int playRound(std::shared_ptr<PlayerAlgorithm> algo1, std::shared_ptr<PlayerAlgorithm> algo2);
private:
    enum class PlayerStatus {
        Playing,
        InvalidPos,
        InvalidMove,
        NoFlags,
        CantMove,
    };
    struct Player {
        Player(int index, std::shared_ptr<PlayerAlgorithm> algo) :
            algo(algo),
            status(PlayerStatus::Playing),
            numFlags(0),
            numMovable(0),
            index(index) {}
        std::shared_ptr<PlayerAlgorithm> algo;
        PlayerStatus status = PlayerStatus::Playing;
        std::map<char, unsigned int> numPieces;
        unsigned int numFlags;
        unsigned int numMovable;
        int index;
    };
    void position(int i, std::vector<std::unique_ptr<FightInfo>>& fights);
    void doMove(int i);
    void changeJoker(int i);
    int output();
    std::unique_ptr<FightInfo> fight(const Point& pos, const Piece& piece1);
    void kill(const Piece& piece);
    bool isValid(const std::unique_ptr<Move>& move, int i) const;
    bool isValid(const std::unique_ptr<JokerChange>& jokerChange, int i) const;
    bool isValid(const std::unique_ptr<PiecePosition>& piecePos, const GameBoard<Piece>& board) const;
    bool isValid(std::unique_ptr<Player>& player) const;
    std::unique_ptr<Player> _players[2];
    GameBoard<Piece> _board;
    unsigned int _numFights;
    const unsigned int FIGHTS_THRESHOLD = 100;
};