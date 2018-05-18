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
			if (Piece::canMove[piece->getType()]) player.numMovable++;
		}
	}
	if (player.numFlags == 0 || player.numMovable == 0) {
		player.status = PlayerStatus::InvalidPos;
		return;
	}
	// TODO: merge boards here?
}

void GameManager::doMove(Player & player) {
	const auto move = player.algo->getMove();
	const auto& from = move->getFrom();
	const auto& to = move->getTo();
	if (_board.getPlayer(from) != player.index || !_board.isValidPosition(to) || _board.getPlayer(to) == player.index) {
		player.status = PlayerStatus::InvalidMove;
		return;
	}
	if (_board.getPlayer(to) != 0) { // fight
		const auto piece1 = _board.getPiece(from);
		const auto piece2 = _board.getPiece(to);
	} else { // no fight
		_board.setPiece(to, _board.getPiece(from));
	}
}

void GameManager::changeJoker(Player & player) {
	(void)player;
}

std::shared_ptr<Piece> fight(std::shared_ptr<Piece> piece1, std::shared_ptr<Piece> piece2) {
	(void)piece1;
	(void)piece2;
	return nullptr;
}

void GameManager::output() {
}

bool GameManager::isGameOn() {
	return _players[0].status == PlayerStatus::Playing && _players[1].status == PlayerStatus::Playing;
}