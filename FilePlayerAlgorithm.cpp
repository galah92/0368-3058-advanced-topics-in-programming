#include <string>
#include "FilePlayerAlgorithm.h"
#include "GamePiecePosition.h"
#include "GameJokerChange.h"
#include "GamePoint.h"
#include "GameMove.h"

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
		ss >> piece >> x >> y >> joker;
        GamePoint pos(x - 1, y - 1);
        positions.push_back(make_unique<GamePiecePosition>(pos, piece, joker));
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
    return make_unique<GameMove>(GamePoint(fromX - 1, fromY - 1), GamePoint(toX - 1, toY - 1));
}

unique_ptr<JokerChange> FilePlayerAlgorithm::getJokerChange() {
    string jokerPrefix;
    _movesstream >> jokerPrefix;
    if (jokerPrefix != "J:") return nullptr;
    int jokerX, jokerY;
    char newRep;
    _movesstream >> jokerX >> jokerY >> newRep;
    GamePoint pos(jokerX, jokerY);
    return make_unique<GameJokerChange>(pos, newRep);
}
