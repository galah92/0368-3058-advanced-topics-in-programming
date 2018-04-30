#include <string>
#include <sstream>
#include "FilePlayerAlgorithm.h"
#include "RPSContainers.h"

using std::string;
using std::to_string;
using std::ostringstream;
using std::istringstream;
using std::make_unique;


const string FILES_PREFIX = "player";
const string BOARD_FILE_EXT	= ".rps_board";
const string MOVES_FILE_EXT	= ".rps_moves";


void FilePlayerAlgorithm::getInitialPositions(int player, vector<unique_ptr<PiecePosition>> &positions) {
    _boardstream = ifstream(FILES_PREFIX + to_string(player + 1) + BOARD_FILE_EXT);
    _movesstream = ifstream(FILES_PREFIX + to_string(player + 1) + MOVES_FILE_EXT);
    positions.clear(); // just to make sure
    string line;
    while (getline(_boardstream, line)) {
		if (line.empty()) continue;
		istringstream ss(line);
		char piece, joker;
		unsigned int x, y;
		ss >> piece >> x >> y >>  joker;
        RPSPoint pos(x, y);
        positions.push_back(make_unique<RPSPiecePosition>(pos, piece, joker));
	}
}

void FilePlayerAlgorithm::notifyOnInitialBoard(const Board& b, const vector<unique_ptr<FightInfo>>& fights) {
    (void)b;
    (void)fights;
}

void FilePlayerAlgorithm::notifyOnOpponentMove(const Move& move) {
    (void)move;
}

void FilePlayerAlgorithm::notifyFightResult(const FightInfo& fightInfo) {
    (void)fightInfo;
}

unique_ptr<Move> FilePlayerAlgorithm::getMove() {
    return nullptr;
}

unique_ptr<JokerChange> FilePlayerAlgorithm::getJokerChange() {
    return nullptr;
}
