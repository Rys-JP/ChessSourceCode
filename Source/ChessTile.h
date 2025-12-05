// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

class AChessPiece;

#include "ChessTile.generated.h"


UCLASS()
class CHESS_API AChessTile : public AActor
{
	GENERATED_BODY()
	
public:	
	AChessTile();

	UPROPERTY()
	TObjectPtr<USceneComponent> DefaultSceneRoot;
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> StaticMesh;

	UPROPERTY(VisibleAnywhere)
	int32 TileNumber;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<AChessPiece> OccupyingPiece;

	void SetColor(int32 Color);

	UFUNCTION()
	void OnClicked(UPrimitiveComponent* TouchedComponent, FKey ButtonPressed);

private:
	UPROPERTY(EditAnywhere)
	TObjectPtr<UMaterialInterface> White;
	UPROPERTY(EditAnywhere)
	TObjectPtr<UMaterialInterface> Black;
	UPROPERTY(EditAnywhere)
	TObjectPtr<UMaterialInterface> Blue;
	UPROPERTY(EditAnywhere)
	TObjectPtr<UMaterialInterface> Green;

	
};
