// Fill out your copyright notice in the Description page of Project Settings.


#include "ChessPiece.h"
#include "ChessTile.h"
#include "ChessBoard.h"
#include "ChessPlayerController.h" 
#include "ChessPawn.h"
#include "MyGameInstance.h"
#include "ChessMode.h"
#include "ChessAIController.h"
#include "Blueprint/UserWidget.h"

#include "Kismet//GameplayStatics.h"


AChessMode::AChessMode()
{
	PlayerControllerClass = AChessPlayerController::StaticClass();
	DefaultPawnClass = nullptr;
	static ConstructorHelpers::FClassFinder<UUserWidget> WidgetClassFinder(TEXT("/Game/Chess/Widget/WBP_Promotion"));
	PromotionWidgetClass = WidgetClassFinder.Class;
}

void AChessMode::BeginPlay() 
{
	Super::BeginPlay();
	GI = Cast<UMyGameInstance>(GetGameInstance());
	UGameplayStatics::GetAllActorsOfClass(this, AChessBoard::StaticClass(), FoundBoards);
	if (FoundBoards.Num() == 0)
		return;
	Board = Cast<AChessBoard>(FoundBoards[0]);

	if (PromotionWidgetClass)
	{
		PromotionWidget = CreateWidget<UUserWidget>(GetWorld(), PromotionWidgetClass);
	}
	//デバッグ用
	if (GI->IsAutoChess)
	{
		PC0 = Cast<AChessPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
		PC0->Team = EPieceTeam::Spectator;
		Spectator = GetWorld()->SpawnActor<AChessPawn>();
		Spectator->Team = EPieceTeam::Spectator;
		PC0->Possess(Spectator);
		PC0->SetViewTargetWithBlend(Spectator);

		AI0 = GetWorld()->SpawnActor<AChessAIController>();
		AI0->Team = EPieceTeam::White;
		WhitePawn = GetWorld()->SpawnActor<AChessPawn>();
		WhitePawn->Team = EPieceTeam::White;
		AI0->Possess(WhitePawn);

		AI1 = GetWorld()->SpawnActor<AChessAIController>();
		AI1->Team = EPieceTeam::Black;
		BlackPawn = GetWorld()->SpawnActor<AChessPawn>();
		BlackPawn->Team = EPieceTeam::Black;
		AI1->Possess(BlackPawn);
	}
	else
	{
		if (GI->IsSinglePlayer) //1人プレイ
		{
			if (GI->IsWhiteSelected) //白選択
			{
				PC0 = Cast<AChessPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
				PC0->Team = EPieceTeam::White;
				WhitePawn = GetWorld()->SpawnActor<AChessPawn>();
				WhitePawn->Team = EPieceTeam::White;
				PC0->Possess(WhitePawn);
				PC0->SetViewTargetWithBlend(WhitePawn);

				AI0 = GetWorld()->SpawnActor<AChessAIController>();
				AI0->Team = EPieceTeam::Black;
				BlackPawn = GetWorld()->SpawnActor<AChessPawn>();
				BlackPawn->Team = EPieceTeam::Black;
				AI0->Possess(BlackPawn);
			}
			else //黒選択
			{
				PC0 = Cast<AChessPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
				PC0->Team = EPieceTeam::Black;
				BlackPawn = GetWorld()->SpawnActor<AChessPawn>();
				BlackPawn->Team = EPieceTeam::Black;
				PC0->Possess(BlackPawn);
				PC0->SetViewTargetWithBlend(BlackPawn);

				AI0 = GetWorld()->SpawnActor<AChessAIController>();
				AI0->Team = EPieceTeam::White;
				WhitePawn = GetWorld()->SpawnActor<AChessPawn>();
				WhitePawn->Team = EPieceTeam::White;
				AI0->Possess(WhitePawn);
			}
		}
		else
		{
			//2人プレイ
		}
	}
	if (AI0)
	{
		AI0->GM = this;
		AI0->GI = GI;
		AI0->Board = Board;
		GI->OnTurnChanged.AddDynamic(AI0, &AChessAIController::OnAITurn);
	}
	if (AI1)
	{
		AI1->GM = this;
		AI1->GI = GI;
		AI1->Board = Board;
		GI->OnTurnChanged.AddDynamic(AI1, &AChessAIController::OnAITurn);
	}
	GI->SwitchTurn();
}

void AChessMode::Move(AChessPiece* Offence, AChessTile* Tile)
{
	GI->OffencePiece = Offence;
	GI->DefencePiece = nullptr;
	GI->OffencePieces.Empty();
	Board->InitializeTileColor();
	Board->InitializePieceColor();
	GI->UpdateBoardState(Offence, Tile->TileNumber);
	Offence->MovePieceToTile(Offence, Tile);
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [=, this]()
	{
		if (Offence->Type == EPieceType::Pawn)
		{
			if ((Offence->Team == EPieceTeam::White && (Tile->TileNumber) / 10 == GI->BoardY) ||
				(Offence->Team == EPieceTeam::Black && (Tile->TileNumber) / 10 == 1))
			{
				CreatePromotionWidget(Offence);
				return;
			}
		}
		Board->UpdateOccupyingPiece();
		Board->CalculateValidMoves();
		GI->SwitchTurn();
	}, Offence->MoveDuration, false);
}

void AChessMode::Battle(AChessPiece* Offence, AChessPiece* Defence, AChessTile* Tile)
{
	//BattleInfo
	GI->OffencePiece = Offence;
	GI->DefencePiece = Defence;
	GI->OffencePieces.Empty();
	for (AChessPiece* Piece : Board->Pieces)
	{
		if (Offence->Team == Piece->Team && Piece->ValidMoves.Contains(Tile) && Piece != Offence)
		{
			GI->OffencePieces.Add(Piece);
		}
	}
	Board->InitializeTileColor();
	Board->InitializePieceColor();
	GI->UpdateBoardState(Offence, Tile->TileNumber);
	Offence->MovePieceToTile(Offence, Tile);
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [=, this]()
	{
		//決闘移行時は勝利判定を消す
		if (Defence->Type == EPieceType::King)
		{
			if (Defence->Team == EPieceTeam::White)
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("Black Win"));
			}
			else
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("White Win"));
			}
		}
		else if (Offence->Type == EPieceType::Pawn)
		{
			if ((Offence->Team == EPieceTeam::White && (Tile->TileNumber) / 10 == GI->BoardY) ||
				(Offence->Team == EPieceTeam::Black && (Tile->TileNumber) / 10 == 1))
			{
				CreatePromotionWidget(Offence);
				//決闘移行時は2行を消す
				GI->UpdateBoardState(Defence);
				Board->Pieces.Remove(Defence);
				Defence->Destroy();
				return;
			}
		}
		//決闘移行時は3行を消す
		GI->UpdateBoardState(Defence);
		Board->Pieces.Remove(Defence);
		Defence->Destroy();
		Board->UpdateOccupyingPiece();
		Board->CalculateValidMoves();
		GI->SwitchTurn();
	}, Offence->MoveDuration, false);
}

void AChessMode::CreatePromotionWidget(AChessPiece* Pawn)
{
	//AIが昇格（1人用）
	if (Pawn->Team != PC0->Team)
	{
		//ランダムに昇格先を選ぶ機能(未実装)
		PromotePawn(EPieceType::Queen);
		return;
	}
	for (AChessPiece* Piece : Board->Pieces)
	{
		Piece->SetActorEnableCollision(false);
	}
	for (AChessTile* Tile : Board->Tiles)
	{
		Tile->SetActorEnableCollision(false);
	}
	PromotionWidget->AddToViewport();
}

void AChessMode::PromotePawn(EPieceType NewType)
{
	for (AChessPiece* Piece : Board->Pieces)
	{
		Piece->SetActorEnableCollision(true);
	}
	for (AChessTile* Tile : Board->Tiles)
	{
		Tile->SetActorEnableCollision(true);
	}
	AChessPiece* Piece = GI->OffencePiece;
	GI->UpdateBoardState(Piece, NewType);
	//検索時にPawnを探すため、Updateより後
	Piece->Type = NewType;
	Piece->SetMeshForType(NewType);
	if (GI->DefencePiece) 
	{
		AChessPiece* DeletePiece = GI->DefencePiece;
		//決闘移行時は勝利判定を消す
		if (DeletePiece->Type == EPieceType::King)
		{
			if (DeletePiece->Team == EPieceTeam::White)
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("Black Win"));
			}
			else
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("White Win"));
			}
		}
		GI->UpdateBoardState(DeletePiece);
		Board->Pieces.Remove(DeletePiece);
		DeletePiece->Destroy();
	}
	Board->UpdateOccupyingPiece();
	Board->CalculateValidMoves();
	GI->SwitchTurn();
}