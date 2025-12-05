// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ChessInfo.h"
#include "EnhancedInputComponent.h"

class AChessPiece;
class AChessBoard;
class AChessTile;
class AChessMode;
class UMyGameInstance;
class AChessPawn;
class UInputMappingContext;
class UInputAction;

#include "ChessPlayerController.generated.h"


UCLASS()
class CHESS_API AChessPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	AChessPlayerController();

	UPROPERTY(EditAnywhere)
	AChessPiece* SelectedPiece;

	UPROPERTY(VisibleAnywhere)
	EPieceTeam Team;

	TArray<AActor*> Boards;
	AChessBoard* Board;

	float DeadZone = 0.6f;

	void OnPieceClicked(AChessPiece* ClickedPiece);
	void OnTileClicked(AChessTile* ClickedTile);
	
protected:
	virtual void BeginPlay() override;
	virtual void SetupInputComponent() override;

private:
	UPROPERTY(EditAnywhere)
	UMyGameInstance* GI;

	UPROPERTY(EditAnywhere)
	AChessMode* GM;

	void OnConfirmTile();
	void OnSelectTile(const FInputActionValue& Value);

	UPROPERTY(VisibleAnywhere)
	UInputMappingContext* IMC;
	UPROPERTY(VisibleAnywhere)
	UInputAction* Confirm;
	UPROPERTY(VisibleAnywhere)
	UInputAction* Select;

	int32 Y;
	int32 X;
	float NextInputTime = 0.0f;
	float InputCoolTime = 0.1f; 
	AChessTile* SelectedTile;
};


