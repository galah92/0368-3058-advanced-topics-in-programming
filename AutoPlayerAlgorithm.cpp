#include <string>
#include "AutoPlayerAlgorithm.h"
#include "Point.h"

void AutoPlayerAlgorithm::getInitialPositions(int player, std::vector<std::unique_ptr<PiecePosition>>& positions) {
	_player = player;
	positions.clear(); // to make sure
					   // random corner?
					   // flag block
	
	positions.push_back(std::make_unique<PiecePositionImpl>(0, 0, 'F', ' ')); // put the flag in the corner
	setPiece(0, 0, std::make_shared<Piece>(player, (PieceType)'F', PieceType::Joker));
	positions.push_back(std::make_unique<PiecePositionImpl>(0, 1, 'B', ' ')); // cover the flag 
	setPiece(0, 1, std::make_shared<Piece>(player, (PieceType)'B', PieceType::Joker));	
	positions.push_back(std::make_unique<PiecePositionImpl>(1, 0, 'B', ' ')); // cover the flag
	setPiece(1, 0, std::make_shared<Piece>(player, (PieceType)'B', PieceType::Joker));
	positions.push_back(std::make_unique<PiecePositionImpl>(1, 1, 'J', 'B')); // cover the flag
	setPiece(1, 1, std::make_shared<Piece>(player, PieceType::Joker, (PieceType)'B'));

																			  // randomize other pieces?
	positions.push_back(std::make_unique<PiecePositionImpl>(5, 5, 'J', 'B'));
	setPiece(5, 5, std::make_shared<Piece>(player, PieceType::Joker, (PieceType)'B'));
	positions.push_back(std::make_unique<PiecePositionImpl>(2, 7, 'R', ' '));
	setPiece(2, 7, std::make_shared<Piece>(player, (PieceType)'R', PieceType::Joker));
	positions.push_back(std::make_unique<PiecePositionImpl>(8, 2, 'R', ' '));
	setPiece(8, 2, std::make_shared<Piece>(player, (PieceType)'R', PieceType::Joker));
	positions.push_back(std::make_unique<PiecePositionImpl>(2, 2, 'P', ' '));
	setPiece(2, 2, std::make_shared<Piece>(player, (PieceType)'P', PieceType::Joker));
	positions.push_back(std::make_unique<PiecePositionImpl>(3, 7, 'P', ' '));
	setPiece(3, 7, std::make_shared<Piece>(player, (PieceType)'P', PieceType::Joker));
	positions.push_back(std::make_unique<PiecePositionImpl>(4, 4, 'P', ' '));
	setPiece(4, 4, std::make_shared<Piece>(player, (PieceType)'P', PieceType::Joker));
	positions.push_back(std::make_unique<PiecePositionImpl>(5, 8, 'P', ' '));
	setPiece(5, 8, std::make_shared<Piece>(player, (PieceType)'P', PieceType::Joker));
	positions.push_back(std::make_unique<PiecePositionImpl>(7, 4, 'P', ' '));
	setPiece(7, 4, std::make_shared<Piece>(player, (PieceType)'P', PieceType::Joker));
	positions.push_back(std::make_unique<PiecePositionImpl>(6, 6, 'S', ' '));
	setPiece(6, 6, std::make_shared<Piece>(player, (PieceType)'S', PieceType::Joker));

}

void AutoPlayerAlgorithm::notifyOnInitialBoard(const Board & b, const std::vector<std::unique_ptr<FightInfo>>& fights) {
	for (unsigned int y = 0; y < M; y++){
		for (unsigned int x = 0; x < N; x++){
			PointImpl pos(x,y);
			const auto piece = getPiece(pos);
			if (b.getPlayer(pos) != _player && piece->getPlayer() == _player){ // player lose a fight
				setPiece(pos, std::make_shared<Piece>(1 - _player, (PieceType)'U', PieceType::Joker)); // Unknown opponent's piece
			}
		}
	}
	for (const auto &fight : fights){
		notifyFightResult(*fight);
	}
}

void AutoPlayerAlgorithm::notifyOnOpponentMove(const Move & move) {
	(void)move;
}

void AutoPlayerAlgorithm::notifyFightResult(const FightInfo & fightInfo) {
	const auto& pos = fightInfo.getPosition();
	const auto player_piece = fightInfo.getPiece(_player); 
	const auto opponent_piece = fightInfo.getPiece(1 - _player); 
	// TODO : handle joker scenario
	if (fightInfo.getWinner() == _player) {
		setPiece(pos, std::make_shared<Piece>(_player, (PieceType)player_piece, PieceType::Joker));	

	} else if (fightInfo.getWinner() == 1 - _player) { // player lose
		auto iter = _piecesOnBoard.find((PieceType)player_piece);
		if (iter != _piecesOnBoard.end()) iter->second--; // update map
		setPiece(pos, std::make_shared<Piece>(1 - _player, (PieceType)opponent_piece, PieceType::Joker));	


	} else { // both player and opponent lose
		auto iter = _piecesOnBoard.find((PieceType)player_piece);
		if (iter != _piecesOnBoard.end()) iter->second--;
		setPiece(pos, Piece::Empty);
	}

}

std::unique_ptr<Move> AutoPlayerAlgorithm::getMove() {
	auto from = getPosToMoveFrom();
	auto to = getBestNeighbor(from);
	return std::make_unique<MoveImpl>(PointImpl(from->getX(), from->getY()), PointImpl(to->getX(), to->getY()));
}

std::unique_ptr<JokerChange> AutoPlayerAlgorithm::getJokerChange() {
	return nullptr;
}

void AutoPlayerAlgorithm::setPiece(int x, int y, std::shared_ptr<Piece> piece){
	_board[x *  M + y] = piece;
}

void AutoPlayerAlgorithm::setPiece(const Point& pos, std::shared_ptr<Piece> piece){
	_board[pos.getX() *  M + pos.getY()] = piece;
}

std::shared_ptr<Piece> AutoPlayerAlgorithm::getPiece(const Point& pos) const {
	return _board[pos.getX() *  M + pos.getY()];
}

std::unique_ptr<PointImpl> AutoPlayerAlgorithm::getPosToMoveFrom(){
	// TODO : implement
	return std::make_unique<PointImpl>(5,5);
}

std::unique_ptr<PointImpl> AutoPlayerAlgorithm::getBestNeighbor(std::unique_ptr<PointImpl>& from){
	// TODO : implement
	(void)from;
	return std::make_unique<PointImpl>(5,5);
}

bool AutoPlayerAlgorithm::isValidPosition(const Point& pos) const {
	int x = pos.getX();
	int y = pos.getY();
	return x >= 0 && x < N && y >= 0 && y < M;
}