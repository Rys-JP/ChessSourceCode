// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ChessTile.h"
#include "ChessPiece.h"
#include "Components/InstancedStaticMeshComponent.h"

class UMyGameInstance;

#include "ChessBoard.generated.h"


UCLASS()
class CHESS_API AChessBoard : public AActor
{
	GENERATED_BODY()

public:
	AChessBoard();

	UPROPERTY(VisibleAnywhere)
	UMyGameInstance* GI;

	int32 BoardY;
	int32 BoardX;
	int32 OffsetY;
	int32 OffsetX;
	float TileSize = 0.96; //0.01-1.00m

	//Tile
	UPROPERTY(VisibleAnywhere)
	TSubclassOf<AChessTile> TileClass;
	TArray<AChessTile*> Tiles;

	void CreateTiles();
	void InitializeTileColor();
	void CalculateValidMoves();
	AChessTile* TileNumberToTile(int32 TileNumber);
	
	//Piece
	UPROPERTY(VisibleAnywhere)
	TSubclassOf<AChessPiece> PieceClass;
	TArray<AChessPiece*> Pieces;

	void CreatePieces();
	void DeletePieces();
	void InitializePieceColor();
	void UpdateOccupyingPiece();
	
protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	TObjectPtr<USceneComponent> DefaultSceneRoot;
	UPROPERTY()
	TObjectPtr<UInstancedStaticMeshComponent> Board;
	UPROPERTY()
	TObjectPtr<UInstancedStaticMeshComponent> Frame;

	FVector GetTileLocation(int TileNumber);

	void CalculateQRB(AChessPiece* Piece, TArray<int32> Moves);
	void CalculateKK(AChessPiece* Piece, TArray<int32> Moves);
	void CalculateP(AChessPiece* Piece);

	void CreateBoardMesh();
};
