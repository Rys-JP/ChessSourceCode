// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "AIController.h"
#include "ChessInfo.h"

class AChessPiece;
class AChessBoard;
class AChessTile;
class AChessMode;
class UMyGameInstance;
class AChessPawn;

#include "ChessAIController.generated.h"


UCLASS()
class CHESS_API AChessAIController : public AAIController
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere)
	AChessBoard* Board;

	UPROPERTY(VisibleAnywhere)
	EPieceTeam Team;

	UPROPERTY(VisibleAnywhere)
	UMyGameInstance* GI;

	UPROPERTY(VisibleAnywhere)
	AChessMode* GM;

	UFUNCTION()
	void OnAITurn();

	void AIMove();

private:
	FTimerHandle AITimerHandle;
	
};
