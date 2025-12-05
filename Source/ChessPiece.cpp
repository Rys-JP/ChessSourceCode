// Fill out your copyright notice in the Description page of Project Settings.


#include "ChessPiece.h"
#include "ChessTile.h"
#include "ChessPlayerController.h"
#include "MyGameInstance.h"
#include "Kismet/GameplayStatics.h"

AChessPiece::AChessPiece()
{
	PrimaryActorTick.bCanEverTick = true;

	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = DefaultSceneRoot;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	StaticMesh->SetupAttachment(RootComponent);

	StaticMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	StaticMesh->SetCollisionObjectType(ECC_Pawn);
	StaticMesh->OnClicked.AddDynamic(this, &AChessPiece::OnClicked);
}

void AChessPiece::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!bIsMoving)
	{
		return;
	}
	MoveTime += DeltaTime;
	float Alpha = FMath::Clamp(MoveTime / MoveDuration, 0.f, 1.f);
	FVector New = FMath::Lerp(Start, End, Alpha);
	SetActorLocation(New);
	if (Alpha >= 1.0f)
	{
		bIsMoving = false;
		SetActorTickEnabled(false);
	}
}

void AChessPiece::MovePieceToTile(AChessPiece* Piece, AChessTile* Tile)
{
	Start = GetActorLocation();
	End = Tile->GetActorLocation();
	MoveTime = 0.0f;
	float Dist = FVector::Distance(Start, End);
	//11->64 = 583cm 0.583f
	MoveDuration = Dist / 1000;
	CurrentTileNumber = Tile->TileNumber;
	if (!bHasMoved)
	{
		bHasMoved = true;
	}
	bIsMoving = true;
	SetActorTickEnabled(true);
}

void AChessPiece::OnClicked(UPrimitiveComponent* TouchedComponent, FKey ButtonPressed)
{
	AChessPlayerController* PC = Cast<AChessPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
	if (PC)
	{
		PC->OnPieceClicked(this); 
	}
}

void AChessPiece::SetMeshForType(EPieceType PieceType)
{
	UStaticMesh* MeshAsset = nullptr;
	switch (PieceType)
	{
	case EPieceType::Pawn:
		MeshAsset = LoadObject<UStaticMesh>(nullptr, TEXT("/Game/Chess/Meshes/SM_Pawn.SM_Pawn"));
		break;
	case EPieceType::Knight:
		MeshAsset = LoadObject<UStaticMesh>(nullptr, TEXT("/Game/Chess/Meshes/SM_Knight.SM_Knight"));
		break;
	case EPieceType::Rook:
		MeshAsset = LoadObject<UStaticMesh>(nullptr, TEXT("/Game/Chess/Meshes/SM_Rook.SM_Rook"));
		break;
	case EPieceType::Bishop:
		MeshAsset = LoadObject<UStaticMesh>(nullptr, TEXT("/Game/Chess/Meshes/SM_Bishop.SM_Bishop"));
		break;
	case EPieceType::Queen:
		MeshAsset = LoadObject<UStaticMesh>(nullptr, TEXT("/Game/Chess/Meshes/SM_Queen.SM_Queen"));
		break;
	case EPieceType::King:
		MeshAsset = LoadObject<UStaticMesh>(nullptr, TEXT("/Game/Chess/Meshes/SM_King.SM_King"));
		break;
	}
	StaticMesh->SetStaticMesh(MeshAsset);
	StaticMesh->SetWorldScale3D(FVector(1.8, 1.8, 1.8));
}

void AChessPiece::SetColor(FString Color)
{
	if (Color == "White")
	{
		StaticMesh->SetMaterial(0, White);
	}
	else if (Color == "Black")
	{
		StaticMesh->SetMaterial(0, Black);
	}	
	else if (Color == "Blue")
	{
		StaticMesh->SetMaterial(0, Blue);
	}	
}