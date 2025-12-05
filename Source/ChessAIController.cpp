// Fill out your copyright notice in the Description page of Project Settings.


#include "ChessAIController.h"
#include "ChessPiece.h"
#include "ChessBoard.h"
#include "MyGameInstance.h"
#include "ChessMode.h"
#include "Kismet/GameplayStatics.h"

void AChessAIController::OnAITurn()
{
	if (GI->CurrentTurn != Team)
	{
		return;
	}
	GetWorld()->GetTimerManager().SetTimer(AITimerHandle, this, &AChessAIController::AIMove, 1.0f, false);
}

void AChessAIController::AIMove()
{
	if (!GI || !GM || !Board)
	{
		return;
	}
	if (GI->CurrentTurn != Team)
	{
		return;
	}
		
	TArray<AChessPiece*> AIPieces;
	int32 PieceIndex;
	int32 MoveIndex;
	AChessPiece* MovePiece;
	AChessTile* TargetTile;
	if (true) 
	{
		//Level1 
		for (AChessPiece* Piece : Board->Pieces)
		{
			if (!Piece) continue;
			if (Piece->Team == Team && Piece->ValidMoves.Num() > 0)
				AIPieces.Add(Piece);
		}
		if (AIPieces.Num() == 0)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("AI Cannot Move"));
			GI->SwitchTurn();
			return;
		}
		PieceIndex = FMath::RandRange(0, AIPieces.Num() - 1);
		MovePiece = AIPieces[PieceIndex];
		MoveIndex = FMath::RandRange(0, MovePiece->ValidMoves.Num() - 1);
		TargetTile = MovePiece->ValidMoves[MoveIndex];
		if (!TargetTile)
		{
			return;
		}
	}
	else
	{
		//Level2
	}

	if (TargetTile->OccupyingPiece)
	{
		GM->Battle(MovePiece, TargetTile->OccupyingPiece, TargetTile);
	}
	else
	{
		GM->Move(MovePiece, TargetTile);
	}
}