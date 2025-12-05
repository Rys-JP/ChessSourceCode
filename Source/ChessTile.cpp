// Fill out your copyright notice in the Description page of Project Settings.


#include "ChessTile.h"
#include "ChessPlayerController.h"
#include "Kismet/GameplayStatics.h"

AChessTile::AChessTile()
{
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	StaticMesh->SetupAttachment(RootComponent);

	StaticMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	StaticMesh->SetCollisionObjectType(ECC_Pawn);
	StaticMesh->OnClicked.AddDynamic(this, &AChessTile::OnClicked);
}

void AChessTile::OnClicked(UPrimitiveComponent* TouchedComponent, FKey ButtonPressed)
{
	AChessPlayerController* PC = Cast<AChessPlayerController>(UGameplayStatics::GetPlayerController(this, 0));
	if (PC)
	{
		PC->OnTileClicked(this);
	}
}

void AChessTile::SetColor(int32 Color)
{
	if (Color == 0)
	{
		StaticMesh->SetMaterial(0, Black);
	}	
	else if (Color == 1)
	{
		StaticMesh->SetMaterial(0, White);
	}
	else if (Color == 2)
	{
		StaticMesh->SetMaterial(0, Blue);
	}	
	else if (Color == 3)
	{
		StaticMesh->SetMaterial(0, Green);
	}
}


