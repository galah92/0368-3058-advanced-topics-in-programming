#include <string>
#include "FilePlayerAlgorithm.h"
#include "GamePiecePosition.h"
#include "GameJokerChange.h"
#include "GamePoint.h"
#include "GameMove.h"


const std::string FILES_PREFIX = "player";
const std::string BOARD_FILE_EXT = ".rps_board";
const std::string MOVES_FILE_EXT = ".rps_moves";


void FilePlayerAlgorithm::getInitialPositions(int player, std::vector<std::unique_ptr<PiecePosition>> &positions) {
	_boardstream = std::ifstream(FILES_PREFIX + std::to_string(player + 1) + BOARD_FILE_EXT);
	_movesstream = std::ifstream(FILES_PREFIX + std::to_string(player + 1) + MOVES_FILE_EXT);
	positions.clear(); // just to make sure
	std::string line;
	while (std::getline(_boardstream, line))
	{
		std::istringstream ss(line);
		char piece, joker;
		int x, y;
		ss >> piece >> x >> y >> joker;
		GamePoint pos(x - 1, y - 1);
		positions.push_back(std::make_unique<GamePiecePosition>(pos, piece, joker));
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

unique_ptr<Move> FilePlayerAlgorithm::getMove() {
	std::string line;
	getline(_movesstream, line);
	_movestream = std::istringstream(line);
	int fromX, fromY, toX, toY;
	_movestream >> fromX >> fromY >> toX >> toY;
	return std::make_unique<GameMove>(GamePoint(fromX - 1, fromY - 1), GamePoint(toX - 1, toY - 1));
}

unique_ptr<JokerChange> FilePlayerAlgorithm::getJokerChange() {
	std::string jokerPrefix;
	_movesstream >> jokerPrefix;
	if (jokerPrefix != "J:") return nullptr;
	int jokerX, jokerY;
	char newRep;
	_movesstream >> jokerX >> jokerY >> newRep;
	GamePoint pos(jokerX, jokerY);
	return std::make_unique<GameJokerChange>(pos, newRep);
}