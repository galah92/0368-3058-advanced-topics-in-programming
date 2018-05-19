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
	// TODO: merge boards somehow
	unsigned int playerIndex = 0;
	while (_numFights < FIGHTS_THRESHOLD) {
		if (isGameOn()) {
			doMove(_players[playerIndex]);
		}
		if (isGameOn()) {
			changeJoker(_players[playerIndex]);
		}
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
		auto isJoker = piecePos->getPiece() == 'J';
		auto pieceType = (PieceType)(isJoker ? piecePos->getJokerRep() : piecePos->getPiece());
		if (_tmpBoard.getPlayer(pos) != 0 || !Piece::isValid(pieceType)) {
			player.status = PlayerStatus::InvalidPos;
			return;
		} else {
			auto piece = _tmpBoard.setPiece(pos, std::make_shared<Piece>(player.index, pieceType, isJoker));
			if (piece->getType() == PieceType::Flag) player.numFlags++;
			if (piece->canMove()) player.numMovable++;
		}
	}
	if (player.numFlags == 0 || player.numMovable == 0) {
		player.status = PlayerStatus::InvalidPos;
		return;
	}
	// TODO: merge boards here?
}

void GameManager::doMove(Player& player) {
	const auto move = player.algo->getMove();
	const auto& from = move->getFrom();
	const auto& to = move->getTo();
	if (_board.getPlayer(from) != player.index || !_board.isValidPosition(to) || _board.getPlayer(to) == player.index) {
		player.status = PlayerStatus::InvalidMove;
		return;
	}
	if (_board.getPlayer(to) != 0) { // fight
		_board.setPiece(to, fight(_board.getPiece(from), _board.getPiece(to)));
	} else { // no fight
		_board.setPiece(to, _board.getPiece(from));
	}
}

void GameManager::changeJoker(Player& player) {
	const auto jokerChange = player.algo->getJokerChange();
	const auto rep = (PieceType)jokerChange->getJokerNewRep();
	const auto piece = _board.getPiece(jokerChange->getJokerChangePosition());
	if (Piece::isValid(rep) || !piece || !piece->isJoker()) {
		player.status = PlayerStatus::InvalidMove;
		return;
	}
	piece->setType(rep);
}

void GameManager::output() {
	std::ofstream fout("rps.output");
	if (_players[0].status == PlayerStatus::Playing || _players[1].status == PlayerStatus::Playing) {
		int winner = _players[0].status == PlayerStatus::Playing ? 1 : 2;
		int loser = 1 - winner;
		fout << "Winner:" << winner << std::endl;
		switch (_players[loser].status) {
		case PlayerStatus::InvalidPos:
			fout << "Bad positioning input for player " << loser << std::endl;
			break;
		case PlayerStatus::InvalidMove:
			fout << "Bad move input for player " << loser << std::endl;
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
		fout << "Winner:" << 0 << std::endl;
		if (_players[0].status == PlayerStatus::InvalidPos) {
			fout << "Bad positioning input for both players" << std::endl;
		} else { // _players[0].status == PlayerStatus::NoFlags
			fout << "Bad move input for both players" << std::endl;
		}
	}
	fout << _board;
}

std::shared_ptr<Piece> GameManager::fight(std::shared_ptr<Piece> piece1, std::shared_ptr<Piece> piece2) {
	auto killPiece1 = piece2->canKill(piece1->getType());
	auto killPiece2 = piece1->canKill(piece2->getType());
	if (killPiece1) {
		auto player = _players[piece1->getPlayer()];
		if (piece1->getType() == PieceType::Flag) player.numFlags--;
		if (piece1->canMove()) player.numMovable--;
	}
	if (killPiece2) {
		auto player = _players[piece2->getPlayer()];
		if (piece2->getType() == PieceType::Flag) player.numFlags--;
		if (piece2->canMove()) player.numMovable--;
	}
	if (killPiece1 && killPiece2) return Piece::Empty;
	return killPiece1 ? piece2 : piece1;
}

bool GameManager::isGameOn() {
	return _players[0].status == PlayerStatus::Playing && _players[1].status == PlayerStatus::Playing;
}