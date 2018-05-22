#include <string>
#include "AutoPlayerAlgorithm.h"


AutoPlayerAlgorithm::AutoPlayerAlgorithm() {
	_piecesOnBoardCount = { // num of pieces currently hard-coded 
		{ PieceType::Flag, 1 }, 
		{ PieceType::Rock, 2 },
		{ PieceType::Paper, 5 },
		{ PieceType::Scissors, 1 },
		{ PieceType::Bomb, 2 },
		{ PieceType::Joker, 2 }
	};
	_piecesOnBoard = { 
		{ PieceType::Flag, {} }, 
		{ PieceType::Rock, {} },
		{ PieceType::Paper, {} },
		{ PieceType::Scissors, {} },
		{ PieceType::Bomb, {} },
		{ PieceType::Joker, {} }
	};
	_movablePieces = {{PieceType::Rock, PieceType::Paper, PieceType::Scissors}};
}

void AutoPlayerAlgorithm::getInitialPositions(int player, std::vector<std::unique_ptr<PiecePosition>>& positions) {
	_player = player;
	positions.clear(); // to make sure
					   // random corner?
					   // flag block
	positions.push_back(std::make_unique<PiecePositionImpl>(0, 0, 'F', ' ')); // put the flag in the corner
	_board[{0, 0}] = std::make_shared<Piece>(player, (PieceType)'F', PieceType::Joker);
	positions.push_back(std::make_unique<PiecePositionImpl>(0, 1, 'B', ' ')); // cover the flag 
	_board[{0, 1}] = std::make_shared<Piece>(player, (PieceType)'B', PieceType::Joker);
	positions.push_back(std::make_unique<PiecePositionImpl>(1, 0, 'B', ' ')); // cover the flag
	_board[{1, 0}] = std::make_shared<Piece>(player, (PieceType)'B', PieceType::Joker);
	positions.push_back(std::make_unique<PiecePositionImpl>(1, 1, 'J', 'B')); // cover the flag
	_board[{1, 1}] = std::make_shared<Piece>(player, PieceType::Joker, (PieceType)'B');
	// randomize other pieces?
	positions.push_back(std::make_unique<PiecePositionImpl>(5, 5, 'J', 'B'));
	_board[{5, 5}] = std::make_shared<Piece>(player, PieceType::Joker, (PieceType)'B');
	positions.push_back(std::make_unique<PiecePositionImpl>(2, 7, 'R', ' '));
	_board[{2, 7}] = std::make_shared<Piece>(player, (PieceType)'R', PieceType::Joker);
	positions.push_back(std::make_unique<PiecePositionImpl>(8, 2, 'R', ' '));
	_board[{8, 2}] = std::make_shared<Piece>(player, (PieceType)'R', PieceType::Joker);
	positions.push_back(std::make_unique<PiecePositionImpl>(2, 2, 'P', ' '));
	_board[{2, 2}] = std::make_shared<Piece>(player, (PieceType)'P', PieceType::Joker);
	positions.push_back(std::make_unique<PiecePositionImpl>(3, 7, 'P', ' '));
	_board[{3, 7}] = std::make_shared<Piece>(player, (PieceType)'P', PieceType::Joker);
	positions.push_back(std::make_unique<PiecePositionImpl>(4, 4, 'P', ' '));
	_board[{4, 4}] = std::make_shared<Piece>(player, (PieceType)'P', PieceType::Joker);
	positions.push_back(std::make_unique<PiecePositionImpl>(5, 8, 'P', ' '));
	_board[{5, 8}] = std::make_shared<Piece>(player, (PieceType)'P', PieceType::Joker);
	positions.push_back(std::make_unique<PiecePositionImpl>(7, 4, 'P', ' '));
	_board[{7, 4}] = std::make_shared<Piece>(player, (PieceType)'P', PieceType::Joker);
	positions.push_back(std::make_unique<PiecePositionImpl>(6, 6, 'S', ' '));
	_board[{6, 6}] = std::make_shared<Piece>(player, (PieceType)'S', PieceType::Joker);
}

void AutoPlayerAlgorithm::notifyOnInitialBoard(const Board& b, const std::vector<std::unique_ptr<FightInfo>>& fights) {
	for (unsigned int y = 0; y < M; y++) {
		for (unsigned int x = 0; x < N; x++) {
			PointImpl pos(x,y);
			const auto piece = _board[pos];
			if (b.getPlayer(pos) != _player && piece->getPlayer() == _player) { // player lose a fight
				_board[pos] = std::make_shared<Piece>(1 - _player, (PieceType)'U', PieceType::Joker); // Unknown opponent's piece
			}
		}
	}
	for (const auto& fight : fights) {
		notifyFightResult(*fight);
	}
}

void AutoPlayerAlgorithm::notifyOnOpponentMove(const Move& move) {
	_board[move.getTo()] = std::make_shared<Piece>(1 - _player, (PieceType)'U', PieceType::Joker); // update board 
	_board[move.getFrom()] = Piece::Empty; // update board
}

void AutoPlayerAlgorithm::notifyFightResult(const FightInfo& fightInfo) {
	const auto& pos = fightInfo.getPosition();
	const auto player_piece = fightInfo.getPiece(_player); 
	const auto opponent_piece = fightInfo.getPiece(1 - _player); 
	if (fightInfo.getWinner() == _player) {
		_board[pos] = std::make_shared<Piece>(_player, (PieceType)player_piece, PieceType::Joker);	

	} else if (fightInfo.getWinner() == 1 - _player) { // player lose
		_piecesOnBoardCount[(PieceType)player_piece]--; // update map
		_board[pos] = std::make_shared<Piece>(1 - _player, (PieceType)opponent_piece, PieceType::Joker);
	} else { // both player and opponent lose
		_piecesOnBoardCount[(PieceType)player_piece]--;
		_board[pos] = Piece::Empty;
	}
}

std::unique_ptr<Move> AutoPlayerAlgorithm::getMove() {
	std::unique_ptr<PointImpl> from;
	std::unique_ptr<PointImpl> to;
	while (true) {
		from = getPosToMoveFrom();
		if (from == nullptr) continue;
		to = getBestNeighbor(from);
		if (to == nullptr) continue;
		break;
	}
	_board[*from] = Piece::Empty; // update board
	if (_board[*to]->getPlayer() != 1 - _player) { // there will be no fight
		_board[*to] = std::make_shared<Piece>(1 - _player, (PieceType)'U', PieceType::Joker); // update board 
	}
	return std::make_unique<MoveImpl>(PointImpl(from->getX(), from->getY()), PointImpl(to->getX(), to->getY()));
}

std::unique_ptr<JokerChange> AutoPlayerAlgorithm::getJokerChange() {
	// currently no need to change Jokers (Jokers are bombs and protect the flag)
	return nullptr;
}

std::unique_ptr<PointImpl> AutoPlayerAlgorithm::getPosToMoveFrom() {
	for (const auto& pieceType : _movablePieces){
		if (_piecesOnBoardCount[pieceType] == 0) continue;
		for (unsigned int y = 0; y < M; y++) {
			for (unsigned int x = 0; x < N; x++) {
				const auto& piece = _board[PointImpl(x,y)];		
				if (piece->getPlayer() != _player) continue;
				return std::make_unique<PointImpl>(x,y);
			}
		}
	}
	return nullptr;
}

std::unique_ptr<PointImpl> AutoPlayerAlgorithm::getBestNeighbor(std::unique_ptr<PointImpl>& from) {
	for (const auto& pos : validPermutations(from)) {
		if (_board[pos]->getPlayer() == _player) continue;
		return std::make_unique<PointImpl>(pos.getX(), pos.getY());
	}
	return nullptr;
}

std::vector<PointImpl> AutoPlayerAlgorithm::validPermutations(std::unique_ptr<PointImpl>& from) {
	std::vector<PointImpl> vec;
	int x = from->getX();
	int y = from->getY();
	if (_board.isValidPosition(PointImpl(x-1, y-1))) vec.push_back(PointImpl(x-1, y-1));
	if (_board.isValidPosition(PointImpl(x-1, y  ))) vec.push_back(PointImpl(x-1, y));
	if (_board.isValidPosition(PointImpl(x-1, y+1))) vec.push_back(PointImpl(x-1, y+1));
	if (_board.isValidPosition(PointImpl(x, y-1  ))) vec.push_back(PointImpl(x, y-1));
	if (_board.isValidPosition(PointImpl(x, y+1  ))) vec.push_back(PointImpl(x, y+1));
	if (_board.isValidPosition(PointImpl(x+1, y-1))) vec.push_back(PointImpl(x+1, y-1));
	if (_board.isValidPosition(PointImpl(x+1, y  ))) vec.push_back(PointImpl(x+1, y));
	if (_board.isValidPosition(PointImpl(x+1, y+1))) vec.push_back(PointImpl(x+1, y+1));
	return vec;
}