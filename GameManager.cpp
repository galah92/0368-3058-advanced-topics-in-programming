#include "GameManager.h"


void GameManager::play(std::shared_ptr<PlayerAlgorithm> algo1, std::shared_ptr<PlayerAlgorithm> algo2) {
	// init
	_board.clear();
	_players[0] = Player(1, algo1);
	_players[1] = Player(2, algo2);
	// positioning
	position(_players[0]);
	position(_players[1]);
	if (_players[0].status != PlayerStatus::Playing || _players[1].status != PlayerStatus::Playing) {
		// TODO: output error
	}
}

void GameManager::position(Player& player) {
	BoardImpl _tmpBoard;
	std::vector<std::unique_ptr<PiecePosition>> positions;
	player.algo->getInitialPositions(player.index, positions);
	for (auto const& piecePos : positions) {
		if (player.status != PlayerStatus::Playing) return;
		auto const& pos = piecePos->getPosition();
		auto isJoker = piecePos->getPiece() == 'J';
		auto pieceType = (PieceType)(isJoker ? piecePos->getJokerRep() : piecePos->getPiece());
		if (_tmpBoard.getPlayer(pos) != 0 || !Piece::isValid(pieceType)) {
			player.status = PlayerStatus::InvalidPos;
		} else {
			auto piece = _tmpBoard.setPiece(pos, std::make_shared<Piece>(player.index, pieceType, isJoker));
			if (piece->getType() == PieceType::Flag) player.numFlags++;
			if (Piece::canMove[piece->getType()]) player.numMovable++;
		}
	}
	if (player.numFlags == 0) player.status == PlayerStatus::NoFlags;
	if (player.numMovable == 0) player.status == PlayerStatus::CantMove;
	if (player.status == PlayerStatus::Playing) {
		// TODO: merge boards
	}
}