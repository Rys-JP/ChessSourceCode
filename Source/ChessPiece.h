// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ChessInfo.h"

class AChessTile;
class UMyGameInstance;

#include "ChessPiece.generated.h"


UCLASS()
class CHESS_API AChessPiece : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AChessPiece();
	
	UPROPERTY()
	TObjectPtr<USceneComponent> DefaultSceneRoot;
	UPROPERTY()
	TObjectPtr<UStaticMeshComponent> StaticMesh;

	//BoardState
	UPROPERTY(VisibleAnywhere)
	EPieceTeam Team;
	UPROPERTY(VisibleAnywhere)
	EPieceType Type;
	UPROPERTY(VisibleAnywhere)
	int32 CurrentTileNumber;
	UPROPERTY(VisibleAnywhere)
	bool bHasMoved;

	UPROPERTY(VisibleAnywhere)
	TArray<AChessTile*> ValidMoves;

	float MoveDuration;

	void MovePieceToTile(AChessPiece* Piece, AChessTile* Tile);
	void SetColor(FString Color);
	void SetMeshForType(EPieceType PieceType);

	UFUNCTION()
	void OnClicked(UPrimitiveComponent* TouchedComponent, FKey ButtonPressed);

protected:
	//BeginPlay‚ð“ü‚ê‚é‚ÆˆÚ“®‚µ‚È‚­‚È‚é
	virtual void Tick(float DeltaTime) override;

private:
	bool bIsMoving = false;
	FVector Start;
	FVector End;
	float MoveTime;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<UMaterialInterface> White;
	UPROPERTY(EditAnywhere)
	TObjectPtr<UMaterialInterface> Black;
	UPROPERTY(EditAnywhere)
	TObjectPtr<UMaterialInterface> Blue;
};
