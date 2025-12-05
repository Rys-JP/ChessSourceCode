// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameInstance.h"

void UMyGameInstance::InitializeBoardState()
{
	BoardState.Empty();
}

void UMyGameInstance::SetupBoardState()
{
	if (true)
	{
		//White
		BoardState.Add(FPieceInfo(EPieceTeam::White, EPieceType::Rook, 11, false));
		BoardState.Add(FPieceInfo(EPieceTeam::White, EPieceType::Queen, 12, false));
		BoardState.Add(FPieceInfo(EPieceTeam::White, EPieceType::King, 13, false));
		BoardState.Add(FPieceInfo(EPieceTeam::White, EPieceType::Bishop, 14, false));
		BoardState.Add(FPieceInfo(EPieceTeam::White, EPieceType::Knight, 21, false));
		BoardState.Add(FPieceInfo(EPieceTeam::White, EPieceType::Pawn, 22, false));
		BoardState.Add(FPieceInfo(EPieceTeam::White, EPieceType::Pawn, 23, false));
		BoardState.Add(FPieceInfo(EPieceTeam::White, EPieceType::Pawn, 24, false));
		//Black
		BoardState.Add(FPieceInfo(EPieceTeam::Black, EPieceType::Rook, 64, false));
		BoardState.Add(FPieceInfo(EPieceTeam::Black, EPieceType::Queen, 62, false));
		BoardState.Add(FPieceInfo(EPieceTeam::Black, EPieceType::King, 63, false));
		BoardState.Add(FPieceInfo(EPieceTeam::Black, EPieceType::Bishop, 61, false));
		BoardState.Add(FPieceInfo(EPieceTeam::Black, EPieceType::Knight, 54, false));
		BoardState.Add(FPieceInfo(EPieceTeam::Black, EPieceType::Pawn, 53, false));
		BoardState.Add(FPieceInfo(EPieceTeam::Black, EPieceType::Pawn, 52, false));
		BoardState.Add(FPieceInfo(EPieceTeam::Black, EPieceType::Pawn, 51, false));
	}
	else
	{
		//White
		BoardState.Add(FPieceInfo(EPieceTeam::White, EPieceType::Rook, 11, false));
		BoardState.Add(FPieceInfo(EPieceTeam::White, EPieceType::Knight, 12, false));
		BoardState.Add(FPieceInfo(EPieceTeam::White, EPieceType::Bishop, 13, false));
		BoardState.Add(FPieceInfo(EPieceTeam::White, EPieceType::Queen, 14, false));
		BoardState.Add(FPieceInfo(EPieceTeam::White, EPieceType::King, 15, false));
		BoardState.Add(FPieceInfo(EPieceTeam::White, EPieceType::Bishop, 16, false));
		BoardState.Add(FPieceInfo(EPieceTeam::White, EPieceType::Knight, 17, false));
		BoardState.Add(FPieceInfo(EPieceTeam::White, EPieceType::Rook, 18, false));
		BoardState.Add(FPieceInfo(EPieceTeam::White, EPieceType::Pawn, 21, false));
		BoardState.Add(FPieceInfo(EPieceTeam::White, EPieceType::Pawn, 22, false));
		BoardState.Add(FPieceInfo(EPieceTeam::White, EPieceType::Pawn, 23, false));
		BoardState.Add(FPieceInfo(EPieceTeam::White, EPieceType::Pawn, 24, false));
		BoardState.Add(FPieceInfo(EPieceTeam::White, EPieceType::Pawn, 25, false));
		BoardState.Add(FPieceInfo(EPieceTeam::White, EPieceType::Pawn, 26, false));
		BoardState.Add(FPieceInfo(EPieceTeam::White, EPieceType::Pawn, 27, false));
		BoardState.Add(FPieceInfo(EPieceTeam::White, EPieceType::Pawn, 28, false));
		//Black
		BoardState.Add(FPieceInfo(EPieceTeam::Black, EPieceType::Rook, 81, false));
		BoardState.Add(FPieceInfo(EPieceTeam::Black, EPieceType::Knight, 82, false));
		BoardState.Add(FPieceInfo(EPieceTeam::Black, EPieceType::Bishop, 83, false));
		BoardState.Add(FPieceInfo(EPieceTeam::Black, EPieceType::Queen, 84, false));
		BoardState.Add(FPieceInfo(EPieceTeam::Black, EPieceType::King, 85, false));
		BoardState.Add(FPieceInfo(EPieceTeam::Black, EPieceType::Bishop, 86, false));
		BoardState.Add(FPieceInfo(EPieceTeam::Black, EPieceType::Knight, 87, false));
		BoardState.Add(FPieceInfo(EPieceTeam::Black, EPieceType::Rook, 88, false));
		BoardState.Add(FPieceInfo(EPieceTeam::Black, EPieceType::Pawn, 71, false));
		BoardState.Add(FPieceInfo(EPieceTeam::Black, EPieceType::Pawn, 72, false));
		BoardState.Add(FPieceInfo(EPieceTeam::Black, EPieceType::Pawn, 73, false));
		BoardState.Add(FPieceInfo(EPieceTeam::Black, EPieceType::Pawn, 74, false));
		BoardState.Add(FPieceInfo(EPieceTeam::Black, EPieceType::Pawn, 75, false));
		BoardState.Add(FPieceInfo(EPieceTeam::Black, EPieceType::Pawn, 76, false));
		BoardState.Add(FPieceInfo(EPieceTeam::Black, EPieceType::Pawn, 77, false));
		BoardState.Add(FPieceInfo(EPieceTeam::Black, EPieceType::Pawn, 78, false));
	}
	
}

void UMyGameInstance::UpdateBoardState(AChessPiece* Piece, int32 New)
{
	for (FPieceInfo& Info : BoardState)
	{
		if (Info.Team == Piece->Team &&
			Info.Type == Piece->Type &&
			Info.TileNumber == Piece->CurrentTileNumber)
		{
			Info.TileNumber = New;
			Info.HasMoved = true;
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, TEXT("Update TileNumber"));
			break;
		}
	}
}

void UMyGameInstance::UpdateBoardState(AChessPiece* Piece, EPieceType New)
{
	for (FPieceInfo& Info : BoardState)
	{
		if (Info.Team == Piece->Team &&
			Info.Type == Piece->Type &&
			Info.TileNumber == Piece->CurrentTileNumber)
		{
			Info.Type = New;
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, TEXT("Update Type"));
			break;
		}
	}
}

void UMyGameInstance::UpdateBoardState(AChessPiece* Piece)
{
	for (int32 i = BoardState.Num() -1; i >=0; i--)
	{
		if (BoardState[i].Team == Piece->Team &&
			BoardState[i].Type == Piece->Type &&
			BoardState[i].TileNumber == Piece->CurrentTileNumber)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::White, TEXT("Remove Piece"));
			BoardState.RemoveAt(i);
			break;
		}
	}
}

void UMyGameInstance::SwitchTurn()
{
	if (CurrentTurn == EPieceTeam::Black)
	{
		CurrentTurn = EPieceTeam::White;
	}
	else
	{
		CurrentTurn = EPieceTeam::Black;
	}
	OnTurnChanged.Broadcast();
}

