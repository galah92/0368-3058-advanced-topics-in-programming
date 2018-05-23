#include <string>
#include "AutoPlayerAlgorithm.h"


AutoPlayerAlgorithm::AutoPlayerAlgorithm() {
	_piecesOnBoardCount = { // num of pieces currently hard-coded 
		{ 'F', 1 }, 
		{ 'R', 2 },
		{ 'P', 5 },
		{ 'S', 1 },
		{ 'B', 2 },
		{ 'J', 2 }
	};
	_piecesOnBoard = { 
		{ 'F', {} }, 
		{ 'R', {} },
		{ 'P', {} },
		{ 'S', {} },
		{ 'B', {} },
		{ 'J', {} }
	};
	_movablePieces = {{'R', 'P', 'S'}};
}

void AutoPlayerAlgorithm::getInitialPositions(int player, std::vector<std::unique_ptr<PiecePosition>>& positions) {
	_player = player;
	_board.clear();
	positions.clear();
	initBoard();
	for (auto i = 0; i < N; i++) {
		for (auto j = 0; j < M; j++) {
			const auto& piece = _board[{i, j}];
			if (piece->getPlayer() == player) {
				positions.push_back(std::make_unique<PiecePositionImpl>(i, j, piece->getType(), piece->getJokerType()));
			}
		}
	}
}

void AutoPlayerAlgorithm::notifyOnInitialBoard(const Board& board, const std::vector<std::unique_ptr<FightInfo>>& fights) {
	for (auto y = 0; y < M; y++) {
		for (auto x = 0; x < N; x++) {
			PointImpl pos(x,y);
			if (board.getPlayer(pos) != _player && _board[pos]->getPlayer() == _player) { // player lose a fight
				_board[pos] = std::make_shared<Piece>(1 - _player, 'U'); // Unknown opponent's piece
			}
		}
	}
	for (const auto& fight : fights) notifyFightResult(*fight);
}

void AutoPlayerAlgorithm::notifyOnOpponentMove(const Move& move) {
	_board[move.getTo()] = std::make_shared<Piece>(1 - _player, 'U'); // update board 
	_board[move.getFrom()] = Piece::Empty; // update board
}

void AutoPlayerAlgorithm::notifyFightResult(const FightInfo& fightInfo) {
	const auto& pos = fightInfo.getPosition();
	const auto player_piece = fightInfo.getPiece(_player);
	const auto opponent_piece = fightInfo.getPiece(1 - _player);
	if (fightInfo.getWinner() == _player) {
		_board[pos] = std::make_shared<Piece>(_player, player_piece);
	} else if (fightInfo.getWinner() == 1 - _player) { // player lose
		_piecesOnBoardCount[player_piece]--; // update map
		_board[pos] = std::make_shared<Piece>(1 - _player, opponent_piece);
	} else { // both player and opponent lose
		_piecesOnBoardCount[player_piece]--;
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
		_board[*to] = std::make_shared<Piece>(1 - _player, 'U'); // update board 
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

void AutoPlayerAlgorithm::initBoard() {
	// flag in edge surrounded by bombs & joker
	_board[{0, 0}] = std::make_shared<Piece>(_player, 'F');
	_board[{0, 1}] = std::make_shared<Piece>(_player, 'B');
	_board[{1, 0}] = std::make_shared<Piece>(_player, 'B');
	_board[{1, 1}] = std::make_shared<Piece>(_player, 'J', 'B');
	// currently all other pieces positions are hardcoded
	_board[{5, 5}] = std::make_shared<Piece>(_player, 'J', 'B');
	_board[{2, 7}] = std::make_shared<Piece>(_player, 'R');
	_board[{8, 2}] = std::make_shared<Piece>(_player, 'R');
	_board[{2, 2}] = std::make_shared<Piece>(_player, 'P');
	_board[{3, 7}] = std::make_shared<Piece>(_player, 'P');
	_board[{4, 4}] = std::make_shared<Piece>(_player, 'P');
	_board[{5, 8}] = std::make_shared<Piece>(_player, 'P');
	_board[{7, 4}] = std::make_shared<Piece>(_player, 'P');
	_board[{6, 6}] = std::make_shared<Piece>(_player, 'S');
	// rotate the board by 90deg - flag can be on any edge
	srand(time(NULL));
	auto n = rand() % 4;
	for (auto i = 0; i < n; i++) rotateBoard();
}

void AutoPlayerAlgorithm::rotateBoard() {
	BoardImpl oldBoard = _board;
	for (auto i = 0; i < N; i++) {
		for (auto j = 0; j < M; j++) {
			_board[{i, j}] = oldBoard[{N - 1 - j, i}];
		}
	}
}