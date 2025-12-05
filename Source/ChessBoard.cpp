// Fill out your copyright notice in the Description page of Project Settings.


#include "ChessBoard.h"
#include "MyGameInstance.h"


AChessBoard::AChessBoard()
{
	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = DefaultSceneRoot;

	Board = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Board"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> BoardMesh(TEXT("/Game/Chess/Meshes/SM_Cube.SM_Cube"));
	Board->SetStaticMesh(BoardMesh.Object);
	Board->SetupAttachment(RootComponent);

	Frame = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Frame"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> FrameMesh(TEXT("/Game/Chess/Meshes/SM_Tile.SM_Tile"));
	Frame->SetStaticMesh(FrameMesh.Object);
	Frame->SetupAttachment(RootComponent);

	static ConstructorHelpers::FObjectFinder<UMaterialInterface> Mat(TEXT("/Game/Chess/Materials/MI_Gray.MI_Gray"));
	if (Mat.Succeeded())
	{
		Frame->SetMaterial(0, Mat.Object);
		Board->SetMaterial(0, Mat.Object);
	}

	static ConstructorHelpers::FClassFinder<AChessTile> TileBP(TEXT("/Game/Chess/Blueprints/BP_ChessTile"));
	if (TileBP.Class)
		TileClass = TileBP.Class;
	static ConstructorHelpers::FClassFinder<AChessPiece> PieceBP(TEXT("/Game/Chess/Blueprints/BP_ChessPiece"));
	if (PieceBP.Class)
		PieceClass = PieceBP.Class;
}

void AChessBoard::BeginPlay()
{
	Super::BeginPlay();
	GI = Cast<UMyGameInstance>(GetGameInstance());
	BoardY = GI->BoardY;
	BoardX = GI->BoardX;
	OffsetY = 50 * (BoardY - 1);
	OffsetX = 50 * (BoardX - 1);
	CreateTiles();
	InitializeTileColor();
	CreatePieces();
	InitializePieceColor();
	CreateBoardMesh();
	UpdateOccupyingPiece();
	CalculateValidMoves();
}

void AChessBoard::CreateBoardMesh()
{
	if (!Board || !Frame)
	{
		UE_LOG(LogTemp, Error, TEXT("Board or Frame is Null "));
		return;
	}
	//Board
	FTransform Front(
		FRotator(0, 0, 0), 
		FVector(OffsetY + 100, 0, 0),
		FVector(1, BoardX + 2, 0.1));
	Board->AddInstance(Front);

	FTransform Back(
		FRotator(0, 0, 0),
		FVector(-OffsetY - 100, 0, 0),
		FVector(1, BoardX + 2, 0.1));
	Board->AddInstance(Back);

	FTransform Right(
		FRotator(0, 0, 0),
		FVector(0, OffsetX + 100, 0),
		FVector(BoardY + 2, 1, 0.1));
	Board->AddInstance(Right);

	FTransform Left(
		FRotator(0, 0, 0),
		FVector(0, -OffsetX - 100, 0),
		FVector(BoardY + 2, 1, 0.1));
	Board->AddInstance(Left);

	//Frame
	if (1 - TileSize <= 0)
	{
		return;
	}
	for (int Y = 0; Y <= BoardY; Y++)
	{
		FVector Loc = FVector(100 * Y - 50 - OffsetY, 0, 0);
		FVector Scale = FVector(1 - TileSize, BoardX, 0.01);
		FTransform Transform(FRotator(0, 0, 0), Loc, Scale);
		Frame->AddInstance(Transform);
	}
	for (int X = 0; X <= BoardX; X++)
	{
		FVector Loc = FVector(0, 100 * X - 50 - OffsetX, 0);
		FVector Scale = FVector(BoardY, 1 - TileSize, 0.01);
		FTransform Transform(FRotator(0, 0, 0), Loc, Scale);
		Frame->AddInstance(Transform);
	}
}

void AChessBoard::CreateTiles()
{
	for (int Y = 1; Y <= BoardY; Y++) 
	{
		for (int X = 1; X <= BoardX; X++)
		{
			//Y軸が左右、X軸が前後
			FVector Loc = FVector(
				100 * (Y-1) - OffsetY, 
				100 * (X-1) - OffsetX, 
				0);
			FVector Scale = FVector(TileSize, TileSize, 0.01);
			FTransform Transform = FTransform(FRotator(0, 0, 0), Loc, Scale);
			AChessTile* Tile = GetWorld()->SpawnActor<AChessTile>(TileClass, Transform);
			//それぞれのタイルに番号を付与
			Tile->TileNumber = Y * 10 + X;
			Tiles.Add(Tile);
		}
	}
}

void AChessBoard::InitializeTileColor()
{
	for (AChessTile* Tile : Tiles)
	{
		int32 TileNumber = Tile->TileNumber;
		int32 Y = TileNumber / 10;
		int32 X = TileNumber % 10;
		//偶数->黒　奇数->白
		Tile->SetColor((Y + X) % 2);
	}
}

void AChessBoard::UpdateOccupyingPiece() 
{
	for (AChessTile* Tile : Tiles)
		if (Tile)
		{
			Tile->OccupyingPiece = nullptr;
		}
			
	for (AChessPiece* Piece : Pieces)
	{
		AChessTile* Tile = TileNumberToTile(Piece->CurrentTileNumber);
		if (Tile)
		{
			Tile->OccupyingPiece = Piece;
		}
	}
}

void AChessBoard::CreatePieces()
{
	GI = Cast<UMyGameInstance>(GetGameInstance());
	if (!GI)
	{
		return;
	}
	TArray<FPieceInfo>& BoardState = GI->BoardState;
	for (const FPieceInfo& Info : BoardState)
	{
		FVector Loc = GetTileLocation(Info.TileNumber);
		FRotator Rot;
		if (Info.Team == EPieceTeam::White) 
		{
			Rot = FRotator(0, 90, 0);
		}
		else if (Info.Team == EPieceTeam::Black)
		{
			Rot = FRotator(0, -90, 0);
		}
		AChessPiece* Piece = GetWorld()->SpawnActor<AChessPiece>(PieceClass, Loc, Rot);
		Pieces.Add(Piece);
		Piece->Team = Info.Team;
		Piece->Type = Info.Type;
		Piece->CurrentTileNumber = Info.TileNumber;
		Piece->bHasMoved = Info.HasMoved;
		Piece->SetMeshForType(Info.Type);
	}
}

FVector AChessBoard::GetTileLocation(int TileNumber)
{
	for (AChessTile* Tile : Tiles)
	{
		if (Tile->TileNumber == TileNumber)
		{
			return FVector(Tile->GetActorLocation());
		}
	}
	return FVector(0, 0, -1000);
}

void AChessBoard::DeletePieces()
{
	for (AChessPiece* Piece : Pieces)
	{
		Piece->Destroy();
	}
	Pieces.Empty();
}

void AChessBoard::CalculateValidMoves()
{
	for (AChessPiece* Piece : Pieces)
	{
		EPieceType Type = Piece->Type;
		Piece->ValidMoves.Empty();
		switch (Type)
		{
		case EPieceType::Pawn:
			CalculateP(Piece);
			break;
		case EPieceType::Knight:
			CalculateKK(Piece, { -21, -19, -12, -8, 8, 12, 19, 21 });
			break;
		case EPieceType::Rook:
			CalculateQRB(Piece, { -10, -1, 1, 10 });
			break;
		case EPieceType::Bishop:
			CalculateQRB(Piece, { -11, -9, 9, 11 });
			break;
		case EPieceType::Queen:
			CalculateQRB(Piece, { -11, -10, -9, -1, 1, 9, 10, 11 });
			break;
		case EPieceType::King:
			CalculateKK(Piece, { -11, -10, -9, -1, 1, 9, 10, 11 });
			break;
		}
	}
}

//Queen, Rook, Bishop
void AChessBoard::CalculateQRB(AChessPiece* Piece, TArray<int32> Moves)
{
	int CurrentTN = Piece->CurrentTileNumber;
	EPieceTeam Team = Piece->Team;
	for (int32 i = 0; i < Moves.Num(); i++)
	{
		int32 Max = FMath::Max(BoardX, BoardY);
		for (int32 j = 1; j < Max; j++)
		{
			int32 Target = CurrentTN + Moves[i] * j;
			int Y = Target / 10;
			int X = Target % 10;
			if (1 <= Y && Y <= BoardY && 1 <= X && X <= BoardX)
			{
				AChessTile* Tile = TileNumberToTile(Target);
				//Null
				if (Tile->OccupyingPiece == nullptr)
				{
					Piece->ValidMoves.Add(Tile);
				}
				//Black
				else if (Tile->OccupyingPiece->Team != Team)
				{
					Piece->ValidMoves.Add(Tile);
					break;
				}
				//White
				else break;
			}
			else break;
		}
	}
}

//King, Knight
void AChessBoard::CalculateKK(AChessPiece* Piece, TArray<int32> Moves)
{
	int CurrentTN = Piece->CurrentTileNumber;
	EPieceTeam Team = Piece->Team;
	for (int32 Move : Moves)
	{
		int32 i = CurrentTN + Move;
		int32 Y = i / 10;
		int32 X = i % 10;
		if (1 <= Y && Y <= BoardY && 1 <= X && X <= BoardX)
		{
			AChessTile* Tile = TileNumberToTile(i);
			if (Tile->OccupyingPiece == nullptr || Tile->OccupyingPiece->Team != Team)
			{
				Piece->ValidMoves.Add(Tile);
			}
		}
	}
}

//Pawn
void AChessBoard::CalculateP(AChessPiece* Piece)
{
	int32 TileNumber = Piece->CurrentTileNumber;
	EPieceTeam Team = Piece->Team;
	bool HasMoved = Piece->bHasMoved;
	TArray<int32> AttackMoves = (Team == EPieceTeam::White) ? TArray<int32>{9, 11} : TArray<int32>{-9, -11};
	AChessTile* Tile;
	//斜め前
	for (int32 Move : AttackMoves)
	{
		int32 i = TileNumber + Move;
		int32 Y = i / 10;
		int32 X = i % 10;
		if (1 <= Y && Y <= BoardY && 1 <= X && X <= BoardX)
		{
			Tile = TileNumberToTile(i);
			if (Tile->OccupyingPiece != nullptr && Tile->OccupyingPiece->Team != Team)
			{
				Piece->ValidMoves.Add(Tile);
			}
		}
	}
	//前方1マス目
	int32 Forward = (Team == EPieceTeam::White) ? 10 : -10;
	int32 ForwardOne = TileNumber + Forward;
	if (1 <= ForwardOne / 10 && ForwardOne / 10 <= BoardY && 1 <= ForwardOne % 10 && ForwardOne % 10 <= BoardX)
	{
		Tile = TileNumberToTile(ForwardOne);
		if (Tile->OccupyingPiece == nullptr)
		{
			Piece->ValidMoves.Add(Tile);
			//前方2マス目
			if (!HasMoved)
			{
				int32 ForwardTwo = TileNumber + Forward * 2;
				if (1 <= ForwardTwo / 10 && ForwardTwo / 10 <= BoardY && 1 <= ForwardTwo % 10 && ForwardTwo % 10 <= BoardX)
				{
					Tile = TileNumberToTile(ForwardTwo);
					if (Tile->OccupyingPiece == nullptr)
					{
						Piece->ValidMoves.Add(Tile);
					}
				}
			}
		}
	}
}

AChessTile* AChessBoard::TileNumberToTile(int32 Number)
{
	for (AChessTile* Tile : Tiles)
	{
		if (Tile->TileNumber == Number)
		{
			return Tile;
		}
	}
	return nullptr;
}

void AChessBoard::InitializePieceColor()
{
	for (AChessPiece* Piece : Pieces)
	{
		if (Piece->Team == EPieceTeam::White) 
		{
			Piece->SetColor("White");
		}
		else
		{
			Piece->SetColor("Black");
		}	
	}
}