#include <string>
#include "FilePlayerAlgorithm.h"
#include "GameContainers.h"

using std::string;
using std::to_string;
using std::ostringstream;
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
		istringstream ss(line);
		char piece, joker;
		int x, y;
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
    string line;
    getline(_movesstream, line);
    _movestream = istringstream(line);
    int fromX, fromY, toX, toY;
    _movestream >> fromX >> fromY >> toX >> toY;
    RPSPoint from(fromX, fromY);
    RPSPoint to(toX, toY);
    return make_unique<RPSMove>(from, to);
}

unique_ptr<JokerChange> FilePlayerAlgorithm::getJokerChange() {
    string jokerPrefix;
    _movesstream >> jokerPrefix;
    if (jokerPrefix != "J:") return nullptr;
    int jokerX, jokerY;
    char newRep;
    _movesstream >> jokerX >> jokerY >> newRep;
    RPSPoint pos(jokerX, jokerY);
    return make_unique<RPSJokerChange>(pos, newRep);
}
