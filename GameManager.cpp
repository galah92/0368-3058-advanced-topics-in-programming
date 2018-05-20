#include <iostream>
#include <fstream>
#include "GameManager.h"


const unsigned int FIGHTS_THRESHOLD = 100;

void GameManager::play(std::shared_ptr<PlayerAlgorithm> algo1, std::shared_ptr<PlayerAlgorithm> algo2) {
	// init
	_board.clear();
	_players[0] = Player(1, algo1);
	_players[1] = Player(2, algo2);
	_numFights = 0;
	// positioning
	position(_players[0]);
	position(_players[1]);
	// moves
	unsigned int playerIndex = 0;
	while (_numFights < FIGHTS_THRESHOLD) {
		if (!isGameOn()) break;
		doMove(_players[playerIndex]);
		if (!isGameOn()) break;
		changeJoker(_players[playerIndex]);
		playerIndex = 1 - playerIndex; // switch player
	}
	output();
}

void GameManager::position(Player& player) {
	BoardImpl _tmpBoard;
	std::vector<std::unique_ptr<PiecePosition>> positions;
	player.algo->getInitialPositions(player.index, positions);
	for (const auto& piecePos : positions) {
		if (player.status != PlayerStatus::Playing) return;
		const auto& pos = piecePos->getPosition();
		auto type = (PieceType)piecePos->getPiece();
		auto jokerType = (PieceType)piecePos->getJokerRep();
		if (_tmpBoard.getPlayer(pos) != 0 || !Piece::isValid(type, jokerType)) {
			player.status = PlayerStatus::InvalidPos;
			return;
		} else {
			auto piece = _tmpBoard.setPiece(pos, std::make_shared<Piece>(player.index, type, jokerType));
			if (piece->getType() == PieceType::Flag) player.numFlags++;
			if (piece->canMove()) player.numMovable++;
		}
	}
	if (player.numFlags == 0 || player.numMovable == 0) {
		player.status = PlayerStatus::InvalidPos;
		return;
	}
	for (unsigned int i = 0; i < N; i++) {
		for (unsigned int j = 0; j < N; j++) {
			PointImpl pos(i, j);
			_board.setPiece(pos, fight(_board.getPiece(pos), _tmpBoard.getPiece(pos)));
		}
	}
}

void GameManager::doMove(Player& player) {
	const auto move = player.algo->getMove();
	const auto& from = move->getFrom();
	const auto& to = move->getTo();
	if (_board.getPlayer(from) != player.index || !_board.isValidPosition(to) || _board.getPlayer(to) == player.index) {
		player.status = PlayerStatus::InvalidMove;
		return;
	}
	_board.setPiece(to, fight(_board.getPiece(from), _board.getPiece(to)));
	_board.setPiece(from, Piece::Empty);
}

void GameManager::changeJoker(Player& player) {
	const auto jokerChange = player.algo->getJokerChange();
	if (!jokerChange) return;
	const auto rep = (PieceType)jokerChange->getJokerNewRep();
	const auto piece = _board.getPiece(jokerChange->getJokerChangePosition());
	if (Piece::isValid(rep) || !piece || piece->getType() != PieceType::Joker) {
		player.status = PlayerStatus::InvalidMove;
		return;
	}
	piece->setJokerType(rep);
}

void GameManager::output() {
	std::ofstream fout("rps.output");
	if (_players[0].status == PlayerStatus::Playing || _players[1].status == PlayerStatus::Playing) {
		int winner = _players[0].status == PlayerStatus::Playing ? 0 : 1;
		int loser = 1 - winner;
		fout << "Winner: " << winner + 1 << std::endl << "Reason: ";
		switch (_players[loser].status) {
		case PlayerStatus::InvalidPos:
			fout << "Bad positioning input for player " << loser + 1 << std::endl;
			break;
		case PlayerStatus::InvalidMove:
			fout << "Bad move input for player " << loser + 1 << std::endl;
			break;
		case PlayerStatus::NoFlags:
			fout << "All flags of the opponent are captured" << std::endl;
			break;
		case PlayerStatus::CantMove:
			fout << "All moving PIECEs of the opponent are eaten" << std::endl;
			break;
		default:
			break;
		}
	} else { // tie
		fout << "Winner:" << 0 << std::endl << "Reason: ";
		if (_players[0].status == PlayerStatus::InvalidPos) {
			fout << "Bad positioning input for both players" << std::endl;
		} else { // _players[0].status == PlayerStatus::NoFlags
			fout << "Bad move input for both players" << std::endl;
		}
	}
	fout << std::endl << _board;
}

std::shared_ptr<Piece> GameManager::fight(std::shared_ptr<Piece> piece1, std::shared_ptr<Piece> piece2) {
	auto killPiece1 = piece2->canKill(piece1->getType());
	auto killPiece2 = piece1->canKill(piece2->getType());
	if (killPiece1 && piece1 != Piece::Empty) {
		auto& player = _players[piece1->getPlayer() - 1];
		if (piece1->getType() == PieceType::Flag) {
			player.numFlags--;
			if (player.numFlags == 0) player.status = PlayerStatus::NoFlags;
		}
		if (piece1->canMove()) {
			player.numMovable--;
			if (player.numMovable == 0) player.status = PlayerStatus::CantMove;
		}
	}
	if (killPiece2 && piece2 != Piece::Empty) {
		auto& player = _players[piece2->getPlayer() - 1];
		if (piece2->getType() == PieceType::Flag) {
			player.numFlags--;
			if (player.numFlags == 0) player.status = PlayerStatus::NoFlags;
		}
		if (piece2->canMove()) {
			player.numMovable--;
			if (player.numMovable == 0) player.status = PlayerStatus::CantMove;
		}
	}
	if (killPiece1 && killPiece2) return Piece::Empty;
	return killPiece1 ? piece2 : piece1;
}

bool GameManager::isGameOn() {
	return _players[0].status == PlayerStatus::Playing && _players[1].status == PlayerStatus::Playing;
}
