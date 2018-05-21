#include <string>
#include "AutoPlayerAlgorithm.h"


void AutoPlayerAlgorithm::getInitialPositions(int player, std::vector<std::unique_ptr<PiecePosition>>& positions) {
	_player = player;
	positions.clear(); // to make sure
					   // random corner?
					   // flag block
	
	positions.push_back(std::make_unique<PiecePositionImpl>(0, 0, 'F', ' ')); // put the flag in the corner
	auto piece = std::make_shared<Piece>(player, (PieceType)'F', PieceType::Joker);
	setPiece(0, 0, piece);
	// _piecesOnBoard[(PieceType)'F'].push_back(piece);
	positions.push_back(std::make_unique<PiecePositionImpl>(0, 1, 'B', ' ')); // cover the flag 
	piece = std::make_shared<Piece>(player, (PieceType)'B', PieceType::Joker);
	setPiece(0, 1, piece);	
	// _piecesOnBoard[(PieceType)'B'].push_back(piece);	
	positions.push_back(std::make_unique<PiecePositionImpl>(1, 0, 'B', ' ')); // cover the flag
	piece = std::make_shared<Piece>(player, (PieceType)'B', PieceType::Joker);
	setPiece(1, 0, piece);
	// _piecesOnBoard[(PieceType)'B'].push_back(piece);	
	positions.push_back(std::make_unique<PiecePositionImpl>(1, 1, 'J', 'B')); // cover the flag
	piece = std::make_shared<Piece>(player, PieceType::Joker, (PieceType)'B');
	setPiece(1, 1, piece);
	// _piecesOnBoard[(PieceType)'J'].push_back(piece);	

																			  // randomize other pieces?
	positions.push_back(std::make_unique<PiecePositionImpl>(5, 5, 'J', 'B'));
	piece = std::make_shared<Piece>(player, PieceType::Joker, (PieceType)'B');
	setPiece(5, 5, piece);
	// _piecesOnBoard[(PieceType)'J'].push_back(piece);	
	positions.push_back(std::make_unique<PiecePositionImpl>(2, 7, 'R', ' '));
	piece = std::make_shared<Piece>(player, (PieceType)'R', PieceType::Joker);
	setPiece(2, 7, piece);
	// _piecesOnBoard[(PieceType)'R'].push_back(piece);	
	positions.push_back(std::make_unique<PiecePositionImpl>(8, 2, 'R', ' '));
	piece = std::make_shared<Piece>(player, (PieceType)'R', PieceType::Joker);
	setPiece(8, 2, piece);
	// _piecesOnBoard[(PieceType)'R'].push_back(piece);	
	positions.push_back(std::make_unique<PiecePositionImpl>(2, 2, 'P', ' '));
	piece = std::make_shared<Piece>(player, (PieceType)'P', PieceType::Joker);
	setPiece(2, 2, piece);
	// _piecesOnBoard[(PieceType)'P'].push_back(piece);	
	positions.push_back(std::make_unique<PiecePositionImpl>(3, 7, 'P', ' '));
	piece = std::make_shared<Piece>(player, (PieceType)'P', PieceType::Joker);
	setPiece(3, 7, piece);
	// _piecesOnBoard[(PieceType)'P'].push_back(piece);	
	positions.push_back(std::make_unique<PiecePositionImpl>(4, 4, 'P', ' '));
	piece = std::make_shared<Piece>(player, (PieceType)'P', PieceType::Joker);
	setPiece(4, 4, piece);
	// _piecesOnBoard[(PieceType)'P'].push_back(piece);	
	positions.push_back(std::make_unique<PiecePositionImpl>(5, 8, 'P', ' '));
	piece = std::make_shared<Piece>(player, (PieceType)'P', PieceType::Joker);
	setPiece(5, 8, piece);
	// _piecesOnBoard[(PieceType)'P'].push_back(piece);	
	positions.push_back(std::make_unique<PiecePositionImpl>(7, 4, 'P', ' '));
	piece = std::make_shared<Piece>(player, (PieceType)'P', PieceType::Joker);
	setPiece(7, 4, piece);
	// _piecesOnBoard[(PieceType)'P'].push_back(piece);	
	positions.push_back(std::make_unique<PiecePositionImpl>(6, 6, 'S', ' '));
	piece = std::make_shared<Piece>(player, (PieceType)'S', PieceType::Joker);
	setPiece(6, 6, piece);
	// _piecesOnBoard[(PieceType)'S'].push_back(piece);	

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
	setPiece(move.getTo(), std::make_shared<Piece>(1 - _player, (PieceType)'U', PieceType::Joker)); // update board 
	setPiece(move.getFrom(), Piece::Empty); // update board
}

void AutoPlayerAlgorithm::notifyFightResult(const FightInfo & fightInfo) {
	// TODO : update _piecesOnBoard
	const auto& pos = fightInfo.getPosition();
	const auto player_piece = fightInfo.getPiece(_player); 
	const auto opponent_piece = fightInfo.getPiece(1 - _player); 
	// TODO : handle joker scenario
	if (fightInfo.getWinner() == _player) {
		setPiece(pos, std::make_shared<Piece>(_player, (PieceType)player_piece, PieceType::Joker));	

	} else if (fightInfo.getWinner() == 1 - _player) { // player lose
		_piecesOnBoardCount[(PieceType)player_piece]--; // update map
		
		setPiece(pos, std::make_shared<Piece>(1 - _player, (PieceType)opponent_piece, PieceType::Joker));	


	} else { // both player and opponent lose
		_piecesOnBoardCount[(PieceType)player_piece]--;
		setPiece(pos, Piece::Empty);
	}

}

std::unique_ptr<Move> AutoPlayerAlgorithm::getMove() {
	auto from = getPosToMoveFrom();
	auto to = getBestNeighbor(from);
	return std::make_unique<MoveImpl>(PointImpl(from->getX(), from->getY()), PointImpl(to->getX(), to->getY()));
}

std::unique_ptr<JokerChange> AutoPlayerAlgorithm::getJokerChange() {
	// currently no need to change Jokers (Jokers are bombs and protect the flag)
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

std::shared_ptr<Piece> AutoPlayerAlgorithm::getPiece(int x, int y) const {
	return _board[x *  M + y];
}

std::unique_ptr<PointImpl> AutoPlayerAlgorithm::getPosToMoveFrom(){
	for (const auto pieceType : _movablePieces){
		if (_piecesOnBoardCount[pieceType] == 0) continue;
		for (unsigned int y = 0; y < M; y++){
			for (unsigned int x = 0; x < N; x++){	
				const auto piece = getPiece(x,y);		
				if (piece->getPlayer() != _player) continue;
				return std::make_unique<PointImpl>(x,y);
			}
		}
	}
	return nullptr;
}

bool AutoPlayerAlgorithm::isValidPosition(const Point& pos) const {
	int x = pos.getX();
	int y = pos.getY();
	return isValidPosition(x,y);
}

bool AutoPlayerAlgorithm::isValidPosition(int x, int y) const{
	return x >= 0 && x < N && y >= 0 && y < M;
}

std::unique_ptr<PointImpl> AutoPlayerAlgorithm::getBestNeighbor(std::unique_ptr<PointImpl>& from){
	for (const auto pos : validPermutations(from)){
		if (getPiece(pos)->getPlayer() == _player) continue;
		return std::make_unique<PointImpl>(pos.getX(), pos.getY());
	}
	return nullptr;
}

std::vector<PointImpl> AutoPlayerAlgorithm::validPermutations(std::unique_ptr<PointImpl>& from){
	std::vector<PointImpl> vec;
	int x = from->getX();
	int y = from->getY();
	if (isValidPosition(x-1, y-1)) vec.push_back(PointImpl(x-1, y-1));
	if (isValidPosition(x-1, y)) vec.push_back(PointImpl(x-1, y));
	if (isValidPosition(x-1, y+1)) vec.push_back(PointImpl(x-1, y+1));
	if (isValidPosition(x, y-1)) vec.push_back(PointImpl(x, y-1));
	if (isValidPosition(x, y+1)) vec.push_back(PointImpl(x, y+1));
	if (isValidPosition(x+1, y-1)) vec.push_back(PointImpl(x+1, y-1));
	if (isValidPosition(x+1, y)) vec.push_back(PointImpl(x+1, y));
	if (isValidPosition(x+1, y+1)) vec.push_back(PointImpl(x+1, y+1));
	return vec;
}