// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"

#include "ChessInfo.h"
#include "ChessPiece.h"

#include "MyGameInstance.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FTurnChanged);

UCLASS()
class CHESS_API UMyGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite)
	bool IsWhiteSelected = true;

	UPROPERTY(BlueprintReadWrite)
	bool IsSinglePlayer = true;

	UPROPERTY(BlueprintReadWrite)
	bool IsAutoChess = false;

	int32 BoardY = 6; //8ˆÈ‰º
	int32 BoardX = 4; //8ˆÈ‰º

	UPROPERTY(EditAnywhere)
	EPieceTeam CurrentTurn = EPieceTeam::Black;

	UPROPERTY()
	FTurnChanged OnTurnChanged;

	UPROPERTY(VisibleAnywhere)
	TArray<FPieceInfo> BoardState;

	UPROPERTY()
	AChessPiece* OffencePiece;
	UPROPERTY()
	TArray<AChessPiece*> OffencePieces;
	UPROPERTY()
	AChessPiece* DefencePiece;

	UFUNCTION(BlueprintCallable)
	void SetupBoardState();

	UFUNCTION(BlueprintCallable)
	void InitializeBoardState();

	void SwitchTurn();

	void UpdateBoardState(AChessPiece* Piece, int32 New);
	void UpdateBoardState(AChessPiece* Piece, EPieceType New);
	void UpdateBoardState(AChessPiece* Piece);

protected:


};

