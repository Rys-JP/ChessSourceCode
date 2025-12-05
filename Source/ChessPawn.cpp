// Fill out your copyright notice in the Description page of Project Settings.

#include "ChessPawn.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "ChessPlayerController.h"
#include "ChessAIController.h"
#include "MyGameInstance.h"


AChessPawn::AChessPawn()
{
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->bDoCollisionTest = false;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);
	Camera->FieldOfView = FOV;
}

void AChessPawn::PossessedBy(AController* NewController)
{
	GI = Cast<UMyGameInstance>(GetGameInstance());
	PC = Cast<AChessPlayerController>(NewController);
	AI = Cast<AChessAIController>(NewController);
	if (Team == EPieceTeam::White)
	{
		SpringArm->SetRelativeRotation(FRotator(-50, 0, 0));
	}
	else if (Team == EPieceTeam::Black)
	{
		SpringArm->SetRelativeRotation(FRotator(-50, 180, 0));
	}
	else
	{
		SpringArm->SetRelativeRotation(FRotator(-40, -90, 0));
	}

	int32 Max = FMath::Max(GI->BoardX, GI->BoardY);
	SpringArm->TargetArmLength = Max * 200 + 200;

}