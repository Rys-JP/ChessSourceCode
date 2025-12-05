// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ChessInfo.generated.h"

UENUM()
enum class EPieceType : uint8
{
	Pawn,
	Rook,
	Knight,
	Bishop,
	Queen,
	King
};

UENUM()
enum class EPieceTeam : uint8
{
	White,
	Black,
	Spectator
};

USTRUCT()
struct FPieceInfo
{
	GENERATED_BODY()

	/*
	UPROPERTY()
	class ABP_ChessPiece* Piece;
	*/

	UPROPERTY(EditAnywhere)
	EPieceTeam Team;
	UPROPERTY(EditAnywhere)
	EPieceType Type;
	UPROPERTY(EditAnywhere)
	int32 TileNumber = 0;
	UPROPERTY(EditAnywhere)
	bool HasMoved = false;

	FPieceInfo()
		: Team(EPieceTeam::White)
		, Type(EPieceType::Pawn)
		, TileNumber(0)
		, HasMoved(false)
	{ }
	FPieceInfo(EPieceTeam InTeam, EPieceType InType, int32 InTileNumber, bool InHasMoved) :
		Team(InTeam),
		Type(InType),
		TileNumber(InTileNumber),
		HasMoved(InHasMoved)
	{ }
};