#include <iostream>
#include <fstream>
#include <vector>
#include "GameManager.h"


const unsigned int FIGHTS_THRESHOLD = 100;

GameManager::GameManager(std::shared_ptr<PlayerAlgorithm> algo1, std::shared_ptr<PlayerAlgorithm> algo2) {
	_players[0] = std::make_unique<Player>(1, algo1);
	_players[1] = std::make_unique<Player>(2, algo2);
}

int GameManager::play_round() {
	// init
	_board.clear();
	_numFights = 0;
	// positioning
    std::vector<std::unique_ptr<FightInfo>> fights;
	position(0, fights);
	position(1, fights);
    _players[0]->algo->notifyOnInitialBoard(_board, fights);
    _players[1]->algo->notifyOnInitialBoard(_board, fights);
	// moves
	auto i = 0;
	while (_numFights < FIGHTS_THRESHOLD) {
		if (!isGameOn()) break;
		doMove(i);
		if (!isGameOn()) break;
		changeJoker(i);
		i = 1 - i; // switch player
	}
	return output();
}

void GameManager::position(int i, std::vector<std::unique_ptr<FightInfo>>& fights) {
    auto& player = _players[i];
	BoardImpl _tmpBoard;
	std::vector<std::unique_ptr<PiecePosition>> positions;
	player->algo->getInitialPositions(player->index, positions);
	for (const auto& piecePos : positions) {
		if (player->status != PlayerStatus::Playing) return;
		const auto& pos = piecePos->getPosition();
		auto type = (PieceType)piecePos->getPiece();
		auto jokerType = (PieceType)piecePos->getJokerRep();
		if (_tmpBoard[pos]->getPlayer() != 0 || !Piece::isValid(type, jokerType)) {
			player->status = PlayerStatus::InvalidPos;
			return;
		} else {
			 _tmpBoard[pos] = std::make_shared<Piece>(player->index, type, jokerType);
			player->numPieces[type]++;
			if (_tmpBoard[pos]->getType() == PieceType::Flag) player->numFlags++;
			if (_tmpBoard[pos]->canMove()) player->numMovable++;
		}
	}
	if (player->numFlags == 0 || player->numMovable == 0) {
		player->status = PlayerStatus::InvalidPos;
		return;
	}
	for (const auto& type : player->numPieces) {
		if (type.second > Piece::maxCapacity(type.first)) {
			player->status = PlayerStatus::InvalidPos;
			return;
		}
	}
	for (unsigned int i = 0; i < N; i++) {
		for (unsigned int j = 0; j < N; j++) {
			PointImpl pos(i, j);
			auto fightInfo = fight(pos, _tmpBoard[pos]);
			if (fightInfo) fights.push_back(std::move(fightInfo));
		}
	}
}

void GameManager::doMove(int i) {
	auto& player = _players[i];
	const auto move = player->algo->getMove();
	const auto& from = move->getFrom();
	const auto& to = move->getTo();
	if (!_board.isValidPosition(from) || _board[from]->getPlayer() != i + 1 || !_board.isValidPosition(to) || _board[to]->getPlayer() == i + 1) {
		player->status = PlayerStatus::InvalidMove;
		return;
	}
	auto fightInfo = fight(to, _board[from]);
	if (fightInfo) {
		_players[0]->algo->notifyFightResult(*fightInfo);
		_players[1]->algo->notifyFightResult(*fightInfo);
	}
	_board[from] = Piece::Empty;
	_players[1 - i]->algo->notifyOnOpponentMove(*move);
}

void GameManager::changeJoker(int i) {
	auto& player = _players[i];
	const auto jokerChange = player->algo->getJokerChange();
	if (!jokerChange) return;
	const auto rep = (PieceType)jokerChange->getJokerNewRep();
	const auto& pos = jokerChange->getJokerChangePosition();
	if (!Piece::isValid(rep) || !_board.isValidPosition(pos)) {
		player->status = PlayerStatus::InvalidMove;
		return;
	}
	const auto piece = _board[pos];
	if (_board[pos]->getPlayer() != i || _board[pos]->getType() != PieceType::Joker) {
		player->status = PlayerStatus::InvalidMove;
		return;
	}
	piece->setJokerType(rep);
}

int GameManager::output() {
	std::ofstream fout("rps.output");
	if (_players[0]->status == PlayerStatus::Playing || _players[1]->status == PlayerStatus::Playing) {
		int winner = _players[0]->status == PlayerStatus::Playing ? 0 : 1;
		int loser = 1 - winner;
		fout << "Winner: " << winner + 1 << std::endl << "Reason: ";
		switch (_players[loser]->status) {
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
        fout << std::endl << _board;
        return winner;
	} else { // tie
		fout << "Winner:" << 0 << std::endl << "Reason: ";
		if (_players[0]->status == PlayerStatus::InvalidPos) {
			fout << "Bad positioning input for both players" << std::endl;
		} else { // _players[0].status == PlayerStatus::NoFlags
			fout << "Bad move input for both players" << std::endl;
		}
        fout << std::endl << _board;
        return 0;
	}
}

std::unique_ptr<FightInfo> GameManager::fight(const Point& pos, const std::shared_ptr<Piece> piece1) {
	auto piece2 = _board[pos];
	auto killPiece1 = piece2->canKill(*piece1);
	auto killPiece2 = piece1->canKill(*piece2);
	if (killPiece1 && piece1 != Piece::Empty) kill(piece1);
	if (killPiece2 && piece2 != Piece::Empty) kill(piece2);
	_board[pos] = killPiece1 && killPiece2 ? Piece::Empty : (killPiece1 ? piece2 : piece1);
	if (piece1 == Piece::Empty || piece2 == Piece::Empty) return nullptr;
	auto winner = killPiece1 && killPiece2 ? 0 : (killPiece1 ? 2 : 1);
	auto ch1 = piece1->getPlayer() == 1 ? piece1 : piece2;
	auto ch2 = piece1->getPlayer() == 2 ? piece1 : piece2;
	return std::make_unique<FightInfoImpl>((const PointImpl&)pos, *ch1, *ch2, winner);
}

void GameManager::kill(std::shared_ptr<Piece> piece) {
	auto& player = _players[piece->getPlayer() - 1];
	player->numPieces[piece->getType()]--;
	if (piece->getType() == PieceType::Flag) {
		player->numFlags--;
		if (player->numFlags == 0) player->status = PlayerStatus::NoFlags;
	}
	if (piece->canMove()) {
		player->numMovable--;
		if (player->numMovable == 0) player->status = PlayerStatus::CantMove;
	}
}

bool GameManager::isGameOn() {
	return _players[0]->status == PlayerStatus::Playing && _players[1]->status == PlayerStatus::Playing;
}