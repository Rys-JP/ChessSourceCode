// Fill out your copyright notice in the Description page of Project Settings.


#include "ChessPlayerController.h"
#include "ChessPiece.h"
#include "ChessBoard.h"
#include "MyGameInstance.h"
#include "ChessMode.h"
#include "ChessPawn.h"

#include "Camera/CameraActor.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"

//Input
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputMappingContext.h"  
#include "InputAction.h"  
#include "Engine/LocalPlayer.h"

AChessPlayerController::AChessPlayerController()
{
	static ConstructorHelpers::FObjectFinder<UInputMappingContext> IMCObj(TEXT("/Game/Chess/Input/IMC_Chess.IMC_Chess"));
	IMC = IMCObj.Object;
	static ConstructorHelpers::FObjectFinder<UInputAction> IAConfirmObj(TEXT("/Game/Chess/Input/IA_Confirm.IA_Confirm"));
	Confirm = IAConfirmObj.Object;
	static ConstructorHelpers::FObjectFinder<UInputAction> IASelectObj(TEXT("/Game/Chess/Input/IA_Select.IA_Select"));
	Select = IASelectObj.Object;
}

void AChessPlayerController::BeginPlay()
{
	Super::BeginPlay();
	if (ULocalPlayer* LP = GetLocalPlayer())
	{
		UEnhancedInputLocalPlayerSubsystem* Subsystem = LP->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
		Subsystem->AddMappingContext(IMC, 0);
	}
	bEnableClickEvents = true;

	FInputModeGameAndUI InputMode;
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	InputMode.SetHideCursorDuringCapture(false);
	SetInputMode(InputMode);
	bShowMouseCursor = true;

	GI = Cast<UMyGameInstance>(GetGameInstance());
	GM = GetWorld()->GetAuthGameMode<AChessMode>();
	TArray<AActor*> FoundBoards;
	UGameplayStatics::GetAllActorsOfClass(this, AChessBoard::StaticClass(), FoundBoards);
	if (FoundBoards.Num() == 0)
	{
		return;
	}
	Board = Cast<AChessBoard>(FoundBoards[0]);

	//パッド選択タイルの初期位置
	Y = (Team == EPieceTeam::White) ? 1 : GI->BoardY;
	X = (Team == EPieceTeam::White) ? 1 : GI->BoardX;
}

void AChessPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	if (UEnhancedInputComponent* EIC = Cast<UEnhancedInputComponent>(InputComponent))
	{
		EIC->BindAction(Confirm, ETriggerEvent::Started, this, &AChessPlayerController::OnConfirmTile);
		EIC->BindAction(Select, ETriggerEvent::Triggered, this, &AChessPlayerController::OnSelectTile);
	}
}

void AChessPlayerController::OnConfirmTile()
{
	OnTileClicked(SelectedTile);
}

void AChessPlayerController::OnSelectTile(const FInputActionValue& Value)
{
	//時間を取得してクールタイムを足し、次の入力はそれ以降なら通す
	float CurrentTime = GetWorld()->GetTimeSeconds();
	if (CurrentTime < NextInputTime)
	{
		return;
	}
	NextInputTime = CurrentTime + InputCoolTime;

	if (SelectedTile)
	{
		int32 TN = SelectedTile->TileNumber;
		SelectedTile->SetColor((TN / 10 + TN % 10) % 2);
	}
	if (SelectedPiece)
	{
		for (AChessTile* Tile : SelectedPiece->ValidMoves)
			Tile->SetColor(2);
	}
	
	float InputY = Value.Get<FVector2D>().Y;
	float InputX = Value.Get<FVector2D>().X;
	if (Team == EPieceTeam::White)
	{
		if (InputY < -DeadZone)
		{
			Y -= 1;
			Y = FMath::Clamp(Y, 1, GI->BoardY);
		}
		else if (DeadZone < InputY)
		{
			Y += 1;
			Y = FMath::Clamp(Y, 1, GI->BoardY);
		}
		if (InputX < -DeadZone)
		{
			X -= 1;
			X = FMath::Clamp(X, 1, GI->BoardX);
		}
		else if (DeadZone < InputX)
		{
			X += 1;
			X = FMath::Clamp(X, 1, GI->BoardX);
		}
	}
	else
	{
		if (Team == EPieceTeam::Black)
		{
			if (InputY < -DeadZone)
			{
				Y += 1;
				Y = FMath::Clamp(Y, 1, GI->BoardY);
			}
			else if (DeadZone < InputY)
			{
				Y -= 1;
				Y = FMath::Clamp(Y, 1, GI->BoardY);
			}
			if (InputX < -DeadZone)
			{
				X += 1;
				X = FMath::Clamp(X, 1, GI->BoardX);
			}
			else if (DeadZone < InputX)
			{
				X -= 1;
				X = FMath::Clamp(X, 1, GI->BoardX);
			}
		}
	}
	SelectedTile = Board->TileNumberToTile(10 * Y + X);
	SelectedTile->SetColor(3);
}

void AChessPlayerController::OnTileClicked(AChessTile* ClickedTile)
{
	if (GI->CurrentTurn != Team)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("Not your Turn"));
		return;
	}
	//White SelcetedPiece切り替え
	if (ClickedTile->OccupyingPiece && ClickedTile->OccupyingPiece->Team == Team)
	{
		if (SelectedPiece)
		{
			for (AChessTile* Tile : SelectedPiece->ValidMoves)
			{
				int32 TN = Tile->TileNumber;
				Tile->SetColor((TN / 10 + TN % 10) % 2);
			}
		}
		Board->InitializePieceColor();
		SelectedPiece = ClickedTile->OccupyingPiece;
		SelectedPiece->SetColor("Blue");
		for (AChessTile* Tile : ClickedTile->OccupyingPiece->ValidMoves)
		{
			Tile->SetColor(2);
		}
		return;
	}
	//Black or Null
	if (!SelectedPiece)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("Select your Piece"));
	}
	else
	{
		if (!SelectedPiece->ValidMoves.Contains(ClickedTile))
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("Out of Range"));
		}
		else
		{
			AChessPiece* Piece = SelectedPiece;
			SelectedPiece = nullptr;
			//Black Battle
			if (ClickedTile->OccupyingPiece != nullptr)
			{
				GM->Battle(Piece, ClickedTile->OccupyingPiece, ClickedTile);
			}
			//None Move
			else
			{
				GM->Move(Piece, ClickedTile);
			}
		}
	}
}

void AChessPlayerController::OnPieceClicked(AChessPiece* ClickedPiece)
{
	if (GI->CurrentTurn != Team)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("Not your Turn"));
		return;
	}
	//White SelectedPiece切り替え
	if (ClickedPiece->Team == Team)
	{
		Board->InitializeTileColor();
		Board->InitializePieceColor();
		SelectedPiece = ClickedPiece;
		SelectedPiece->SetColor("Blue");
		for (AChessTile* Tile : ClickedPiece->ValidMoves)
		{
			Tile->SetColor(2);
		}
		return;
	}
	//Black
	if (!SelectedPiece)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("Not your Piece"));
	}
	else
	{
		AChessTile* Tile = Board->TileNumberToTile(ClickedPiece->CurrentTileNumber);
		//Out of Range
		if (!SelectedPiece->ValidMoves.Contains(Tile))
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("Out of Range"));
		}	
		//Battle
		else
		{
			AChessPiece* Piece = SelectedPiece;
			SelectedPiece = nullptr;
			GM->Battle(Piece, Tile->OccupyingPiece, Tile);
		}
	}
}

