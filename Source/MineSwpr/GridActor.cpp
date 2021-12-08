// Fill out your copyright notice in the Description page of Project Settings.


#include "GridActor.h"
#include "Components/TextRenderComponent.h"
#include "BlockActor.h"

#define LOCTEXT_NAMESPACE "PuzzleBlockGrid"

// Sets default values
AGridActor::AGridActor() : IsInGameOverState(false)
{
	// Create dummy root scene component
	DummyRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Dummy0"));
	RootComponent = DummyRoot;

	// Create static mesh component
	ScoreText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("ScoreText0"));
	ScoreText->SetRelativeLocation(FVector(200.f,0.f,0.f));
	ScoreText->SetRelativeRotation(FRotator(90.f,0.f,0.f));
	ScoreText->SetText(FText::Format(LOCTEXT("ScoreFmt", "Score: {0}"), FText::AsNumber(0)));
	ScoreText->SetupAttachment(DummyRoot);

	// Set defaults
	Size = 8;
	BlockSpacing = 150.0f;

	CreateSequenceBlockValues();
}

void AGridActor::CreateRandomMinesPositions()
{
	
	bool IsFindNewPos = false;
	for (int i = 0; i < NUMBER_MINES; i++)
	{
		while (!IsFindNewPos)
		{
			// Search new random but not repeat positions.
			IsFindNewPos = true;
			Candidate = FMath::RandRange(0, SIZE_SECUENCE - 1);
			for (int j = 0; j < i; j++)
			{
				if (MinesPositions[j] == Candidate)
				{
					IsFindNewPos = false;
					break;
				}
			}
		}
		MinesPositions[i] = Candidate;
		IsFindNewPos = false;
	}
}

bool AGridActor::IsMinePosition(int Position)
{
	bool IsFinded = false;
	for (int i = 0; i < NUMBER_MINES; i++)
	{
		if (MinesPositions[i] == Position)
		{
			IsFinded = true;
			break;
		}
	}
	return IsFinded;
}

int AGridActor::NumberRow(int IndexBlock)
{
	return IndexBlock / SIZE_SQUARE;
}

int AGridActor::NumberCol(int IndexBlock)
{
	return IndexBlock % SIZE_SQUARE;
}

int AGridActor::CalcIndexBlock(int Row, int Col)
{
	return (SIZE_SQUARE * Row) + Col;
}

bool AGridActor::IsAMinePosition(int Position)
{
	int Row = NumberRow(Position), RowMine;
	int Col = NumberCol(Position), ColMine;

	for (int i = 0; i < NUMBER_MINES; i++)
	{
		RowMine = NumberRow(MinesPositions[i]);
		ColMine = NumberCol(MinesPositions[i]);
		
		if (((Row == RowMine - 1) && (Col == ColMine - 1)) || ((Row == RowMine - 1) && (Col == ColMine))
				|| ((Row == RowMine - 1) && (Col == ColMine + 1)) || ((Row == RowMine) && (Col == ColMine - 1))
				|| ((Row == RowMine) && (Col == ColMine + 1)) || ((Row == RowMine + 1) && (Col == ColMine - 1))
				|| ((Row == RowMine + 1) && (Col == ColMine)) || ((Row == RowMine + 1) && (Col == ColMine + 1)))
			return true;
	}

	return false;
}

int AGridActor::NumberAMinePosition(int Position)
{
	int Row = NumberRow(Position), RowMine;
	int Col = NumberCol(Position), ColMine;
	int MinesCounter = 0;

	for (int i = 0; i < NUMBER_MINES; i++)
	{
		RowMine = NumberRow(MinesPositions[i]);
		ColMine = NumberCol(MinesPositions[i]);

		if (((Row == RowMine - 1) && (Col == ColMine - 1)) || ((Row == RowMine - 1) && (Col == ColMine))
			|| ((Row == RowMine - 1) && (Col == ColMine + 1)) || ((Row == RowMine) && (Col == ColMine - 1))
			|| ((Row == RowMine) && (Col == ColMine + 1)) || ((Row == RowMine + 1) && (Col == ColMine - 1))
			|| ((Row == RowMine + 1) && (Col == ColMine)) || ((Row == RowMine + 1) && (Col == ColMine + 1)))
		{
			MinesCounter++;
		}
	}

	return MinesCounter;
}

void AGridActor::CreateSequenceBlockValues()
{
	// Create NUMBER_MINES (10) mines in random position in array.
	CreateRandomMinesPositions();

	// Create 8 numbers arround mines (not write in a mine square).
	for (int BlockIndex = 0; BlockIndex < SIZE_SECUENCE; BlockIndex++)
	{
		if (IsMinePosition(BlockIndex))
			SecuenceBlockValue[BlockIndex] = -1;
		else if (IsAMinePosition(BlockIndex))
			SecuenceBlockValue[BlockIndex] = NumberAMinePosition(BlockIndex);
	}
}

void AGridActor::BeginPlay()
{
	Super::BeginPlay();

	// Number of blocks
	const int32 NumBlocks = Size * Size;

	// Loop to spawn each block
	for(int32 BlockIndex=0; BlockIndex<NumBlocks; BlockIndex++)
	{
		const float XOffset = (BlockIndex/Size) * BlockSpacing; // Divide by dimension
		const float YOffset = (BlockIndex%Size) * BlockSpacing; // Modulo gives remainder

		// Make position vector, offset from Grid location
		const FVector BlockLocation = FVector(XOffset, YOffset, 0.f) + GetActorLocation();

		// Spawn a block
		ABlockActor* NewBlock = GetWorld()->SpawnActor<ABlockActor>(BlockLocation, FRotator(0,0,0));

		// Tell the block about its owner
		if (NewBlock != nullptr)
		{
			NewBlock->OwningGrid = this;
			//NewBlock->SetMineValue(FMath::RandRange(-1, 8));
			NewBlock->SetMineValue(SecuenceBlockValue[BlockIndex]);
		}
	}
}


void AGridActor::AddScore()
{
	// Increment score
	Score++;

	// Update text
	ScoreText->SetText(FText::Format(LOCTEXT("ScoreFmt", "Score: {0}"), FText::AsNumber(Score)));
}

void AGridActor::SetGameOverState()
{
	IsInGameOverState = true;
}

bool AGridActor::IsInGameOver() const
{
	return IsInGameOverState;
}

#undef LOCTEXT_NAMESPACE
