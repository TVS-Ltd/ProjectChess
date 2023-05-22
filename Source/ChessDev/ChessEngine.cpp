// Fill out your copyright notice in the Description page of Project Settings.


#include "ChessEngine.h"

// Sets default values
AChessEngine::AChessEngine()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AChessEngine::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AChessEngine::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AChessEngine::setBoardPosition(const FString& shortFen, uint8 enPassant, bool whiteLongCastling, bool whiteShortCastling, bool blackLongCastling, bool blackShortCastling, float moveCtr)
{
	this->boardPosition = BoardPosition(
		TCHAR_TO_UTF8(*shortFen),
		(uint8_t)enPassant,
		whiteLongCastling,
		whiteShortCastling,
		blackLongCastling,
		blackShortCastling,
		moveCtr
	);
}

bool AChessEngine::isGameFinished() const
{
	if (this->whiteVictory() || this->blackVictory() || this->draw())
	{
		return true;
	}

	return false;
}

bool AChessEngine::whiteVictory() const
{
	bool blackHaventGotMoves = (LegalMoveGen::generate(this->boardPosition, Pieces::Black).size() == 0);
    bool blackInCheck = PsLegalMoveMaskGen::inDanger(this->boardPosition.pieces, bsf(this->boardPosition.pieces.pieceBitboards[Pieces::Black][Pieces::King]), Pieces::Black);

    if (blackHaventGotMoves && blackInCheck)
    {
        if(GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, TEXT("White won!"));
		}
    }

    return (blackHaventGotMoves && blackInCheck);
}

bool AChessEngine::blackVictory() const
{
	bool whiteHaventGotMoves = (LegalMoveGen::generate(this->boardPosition, Pieces::White).size() == 0);
    bool whiteInCheck = PsLegalMoveMaskGen::inDanger(this->boardPosition.pieces, bsf(this->boardPosition.pieces.pieceBitboards[Pieces::White][Pieces::King]), Pieces::White);

	if (whiteHaventGotMoves && whiteInCheck)
	{
		if(GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Green, TEXT("Black won!"));
		}
	}

	return (whiteHaventGotMoves && whiteInCheck);
}

bool AChessEngine::draw() const
{
	bool whiteHaventGotMoves = (LegalMoveGen::generate(this->boardPosition, Pieces::White).size() == 0);
	bool whiteInCheck = PsLegalMoveMaskGen::inDanger(this->boardPosition.pieces, bsf(this->boardPosition.pieces.pieceBitboards[Pieces::White][Pieces::King]), Pieces::White);

	bool blackHaventGotMoves = (LegalMoveGen::generate(this->boardPosition, Pieces::Black).size() == 0);
	bool blackInCheck = PsLegalMoveMaskGen::inDanger(this->boardPosition.pieces, bsf(this->boardPosition.pieces.pieceBitboards[Pieces::Black][Pieces::King]), Pieces::Black);

	bool fiftyMovesRule = (this->boardPosition.fiftyMovesCtr >= 50);
	bool threeMovesRule = (this->boardPosition.repetitionHistory.getRepetionNumber(this->boardPosition.hash) >= 3);

	bool whiteMove = this->isWhiteMove();
	bool blackMove = !whiteMove;

	if (whiteHaventGotMoves && !whiteInCheck && whiteMove)
	{
		if(GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("[DRAW] White haven't got moves."));
		}

		return true;
	}

	if (blackHaventGotMoves && !blackInCheck && blackMove)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("[DRAW] Black haven't got moves."));
		}

		return true;
	}

	if (fiftyMovesRule)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("[DRAW] Fifty moves rule."));
		}

		return true;
	}

	if (threeMovesRule)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("[DRAW] Three moves rule."));
		}

		return true;
	}

	return false;
}

bool AChessEngine::isWhiteMove() const
{
	return (this->boardPosition.MoveCtr - floor(this->boardPosition.MoveCtr) < 1e-7);
}

bool AChessEngine::isBlackMove() const
{
	return !this->isWhiteMove();
}

bool AChessEngine::makeMove(FIntPoint from, FIntPoint to, uint8 side, uint8 promotionPiece)
{
	this->moves = LegalMoveGen::generate(this->boardPosition, side);

	ChessMove currMove;
	currMove.From = from.Y + from.X * 8;
	currMove.To = to.Y + to.X * 8;

	this->playerMove = currMove;

	bool moveFound = false;

	for (uint8_t i = 0; i < moves.size(); i++)
	{
		if (moves[i].From == playerMove.From && moves[i].To == playerMove.To && (moves[i].Flag != ChessMove::Flag::PromoteToKnight && moves[i].Flag != ChessMove::Flag::PromoteToBishop && moves[i].Flag != ChessMove::Flag::PromoteToRook && moves[i].Flag != ChessMove::Flag::PromoteToQueen))
		{
			move = moves[i];
			moveFound = true;

			break;
		} else 
		if (moves[i].From == playerMove.From && moves[i].To == playerMove.To)
		{
			switch (promotionPiece)
			{
				case 1:
					move = moves[i];
					move.Flag = ChessMove::Flag::PromoteToKnight;
					moveFound = true;
				break;

				case 2:
					move = moves[i];
					move.Flag = ChessMove::Flag::PromoteToBishop;
					moveFound = true;
				break;

				case 3:
					move = moves[i];
					move.Flag = ChessMove::Flag::PromoteToRook;
					moveFound = true;
				break;

				case 4:
					move = moves[i];
					move.Flag = ChessMove::Flag::PromoteToQueen;
					moveFound = true;
				break;

				default:
					if (GEngine)
					{
						GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("[ERROR] Invalid promotion piece!"));
					}
					
					continue;
				break;
			}

			break;
		}
	}

	if (!moveFound)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("[ERROR] Illegal move!"));
		}
		
		return false;
	}

	this->boardPosition.move(move);

	if (move.DefenderType != 255)
	{
		switch (move.DefenderType)
		{
		case 0:
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Pawn has been captured!"));
			}

			break;

		case 1:
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Knight has been captured!"));
			}

			break;

		case 2:
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Bishop has been captured!"));
			}

			break;

		case 3:
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Rook has been captured!"));
			}

			break;

		case 4:
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Queen has been captured!"));
			}

			break;

		default:
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("[ERROR] Unknown piece type!"));
			}

			break;
		}
	}

	return true;
}

void AChessEngine::makeAIMove(FIntPoint& from, FIntPoint& to, uint8& promotionPiece)
{
	this->move = ai.bestMove(boardPosition, 1, 0, 1000);

	this->boardPosition.move(move);

	if (move.DefenderType != 255)
	{
		switch (move.DefenderType)
		{
		case 0:
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Pawn has been captured!"));
			}

			break;

		case 1:
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Knight has been captured!"));
			}

			break;

		case 2:
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Bishop has been captured!"));
			}

			break;

		case 3:
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Rook has been captured!"));
			}

			break;

		case 4:
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Yellow, TEXT("Queen has been captured!"));
			}

			break;

		default:
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, TEXT("[ERROR] Unknown piece type!"));
			}

			break;
		}
	}

	from = FIntPoint(move.From / 8, move.From % 8);
	to = FIntPoint(move.To / 8, move.To % 8);

	if (move.Flag > 6)
	{
		promotionPiece = int8(move.Flag % 6);
	} else
	{
		promotionPiece = int8(0);
	}
}

bool AChessEngine::makeCCGMove(FIntPoint from, FIntPoint to, uint8 side, uint8 promotionPiece, uint8 pieceToSpawn)
{
	if (pieceToSpawn != 0)
	{
		switch (pieceToSpawn)
		{
		case 1:
			this->boardPosition.addPiece(to.Y + to.X * 8, Pieces::Pawn, side);
			break;

		case 2:
			this->boardPosition.addPiece(to.Y + to.X * 8, Pieces::Knight, side);
			break;

		case 3:
			this->boardPosition.addPiece(to.Y + to.X * 8, Pieces::Bishop, side);
			break;

		case 4:
			this->boardPosition.addPiece(to.Y + to.X * 8, Pieces::Rook, side);
			break;

		case 5:
			this->boardPosition.addPiece(to.Y + to.X * 8, Pieces::Queen, side);
			break;
		
		default:
			return false;
			break;
		}

		return true;
	}

	return this->makeMove(from, to, side, promotionPiece);
}