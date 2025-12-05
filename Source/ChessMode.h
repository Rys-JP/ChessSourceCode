// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ChessInfo.h" 

class AChessPiece;
class AChessTile;
class AChessBoard;
class UMyGameInstance;
class AChessPawn;
class AChessPlayerController;
class AChessAIController;

#include "ChessMode.generated.h"


UCLASS()
class CHESS_API AChessMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AChessMode();

	UPROPERTY(EditAnywhere)
	UMyGameInstance* GI;

	TArray<AActor*> FoundBoards;
	AChessBoard* Board;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> PromotionWidgetClass;
	UUserWidget* PromotionWidget;

	UFUNCTION(BlueprintCallable)
	void PromotePawn(EPieceType NewType);

	void Move(AChessPiece* Offence, AChessTile* Tile);
	void Battle(AChessPiece* Offence, AChessPiece* Defence, AChessTile* Tile);

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere)
	AChessPlayerController* PC0;

	UPROPERTY(VisibleAnywhere)
	AChessPlayerController* PC1;

	UPROPERTY(VisibleAnywhere)
	AChessAIController* AI0;

	UPROPERTY(VisibleAnywhere)
	AChessAIController* AI1;

	UPROPERTY(VisibleAnywhere)
	AChessPawn* WhitePawn;

	UPROPERTY(VisibleAnywhere)
	AChessPawn* BlackPawn;

	UPROPERTY(VisibleAnywhere)
	AChessPawn* Spectator;

	void CreatePromotionWidget(AChessPiece* Pawn);
};

