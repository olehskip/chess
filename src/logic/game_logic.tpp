#ifndef GAME_LOGIC_TPP
#define GAME_LOGIC_TPP

template<typename T>
bool Logic::GameLogic::makePromotion()
{
	// Only these to these piece types pieces can be promoted to
	if(!std::is_same_v<T, Queen> && !std::is_same_v<T, Rook> && !std::is_same_v<T, Bishop> && !std::is_same_v<T, Knight>)
		return false;

	// if the logic doesn't expect promotion, but the board mistakenly wants to do it
	if(gameStatus != GameStatus::PROMOTION_REQUIRED || !promotionRelPos.has_value())
		return false;

	auto piece = getPiece(promotionRelPos.value());
	if(piece.has_value() && checkPieceType<Pawn>(**piece.value())) {
		const auto color = (*piece.value())->color;
		pieces.erase(piece.value());
		pieces.emplace_back(new T(color, promotionRelPos.value()));
		gameStatus = GameStatus::ACTIVE;
		promotionRelPos = std::nullopt;

		checkForCheckmate();
		checkForStalemate();
		return true;
	}

	return false;
}

template<class T>
bool Logic::GameLogic::checkPieceType(const Piece &piece)
{
	return dynamic_cast<const T*>(&piece) != nullptr;
}

#endif // GAME_LOGIC_TPP
