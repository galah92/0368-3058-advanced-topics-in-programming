#include <string>
#include "FilePlayerAlgorithm.h"
#include "GameContainers.h"


const std::string PREFIX = "player";
const std::string BOARD_FILE = ".rps_board";
const std::string MOVES_FILE = ".rps_moves";


void FilePlayerAlgorithm::getInitialPositions(int player, std::vector<std::unique_ptr<PiecePosition>> &positions) {
    // fill positions
    std::ifstream _boardstream(PREFIX + std::to_string(player) + BOARD_FILE);
    positions.clear(); // just to be safe
    std::string line;
    while (std::getline(_boardstream, line))
    {
        std::istringstream iss(line);
        positions.push_back(std::make_unique<PiecePositionImpl>(iss));
    }
    // fill _moves & _jokerChanges
    std::ifstream _movesstream(PREFIX + std::to_string(player) + MOVES_FILE);
    while (std::getline(_movesstream, line))
    {
        std::istringstream iss(line);
        _moves.push_back(std::make_unique<GameMove>(iss));
        std::string token;
        iss >> token;
        _jokerChanges.push_back(token == "J:" ? std::make_unique<GameJokerChange>(iss) : nullptr);
    }
}

void FilePlayerAlgorithm::notifyOnInitialBoard(const Board& b, const std::vector<std::unique_ptr<FightInfo>>& fights) {
    (void)b;
    (void)fights;
}

void FilePlayerAlgorithm::notifyOnOpponentMove(const Move& move) {
    (void)move;
}

void FilePlayerAlgorithm::notifyFightResult(const FightInfo& fightInfo) {
    (void)fightInfo;
}

std::unique_ptr<Move> FilePlayerAlgorithm::getMove() {
    if (_moves.empty()) return nullptr;
    auto move = std::move(_moves.front());
    _moves.pop_front();
    return move;
}

std::unique_ptr<JokerChange> FilePlayerAlgorithm::getJokerChange() {
    if (_jokerChanges.empty()) return nullptr;
    auto jokerChange = std::move(_jokerChanges.front());
    _jokerChanges.pop_front();
    return jokerChange;
}