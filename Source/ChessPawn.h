// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "ChessInfo.h"

class USpringArmComponent;
class UCameraComponent;
class AChessPlayerController;
class AChessAIController;
class UMyGameInstance;

#include "ChessPawn.generated.h"


UCLASS()
class CHESS_API AChessPawn : public APawn
{
	GENERATED_BODY()

public:
	AChessPawn();

	UPROPERTY(VisibleAnywhere)
	UMyGameInstance* GI;

	UPROPERTY(VisibleAnywhere)
	EPieceTeam Team;

	int32 ArmLength = 1200;
	FRotator View = FRotator(-40, -20, 0);
	int32 FOV = 45;

protected:
	virtual void PossessedBy(AController* NewController) override;

private:
	UPROPERTY()
	TObjectPtr<USceneComponent> Root;
	UPROPERTY()
	TObjectPtr<USpringArmComponent> SpringArm;
	UPROPERTY()
	TObjectPtr<UCameraComponent> Camera;

	UPROPERTY(VisibleAnywhere)
	AChessPlayerController* PC;
	UPROPERTY(VisibleAnywhere)
	AChessAIController* AI;


};