// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GridActor.generated.h"

const int SIZE_SQUARE = 8;
const int SIZE_SECUENCE = SIZE_SQUARE * SIZE_SQUARE;
const int NUMBER_MINES = 10;

UCLASS()
class MINESWPR_API AGridActor : public AActor
{
	GENERATED_BODY()

	/** Dummy root component */
	UPROPERTY(Category = Grid, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class USceneComponent* DummyRoot;

	/** Text component for the score */
	UPROPERTY(Category = Grid, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UTextRenderComponent* ScoreText;

	UPROPERTY()
	int Candidate;

public:
	AGridActor();

	/** How many blocks have been clicked */
	int32 Score;

	/** Number of blocks along each side of grid */
	UPROPERTY(Category=Grid, EditAnywhere, BlueprintReadOnly)
	int32 Size;

	/** Spacing of blocks */
	UPROPERTY(Category=Grid, EditAnywhere, BlueprintReadOnly)
	float BlockSpacing;


protected:
	// Begin AActor interface
	virtual void BeginPlay() override;
	// End AActor interface

public:

	/** Handle the block being clicked */
	void AddScore();

	/** Returns DummyRoot subobject **/
	FORCEINLINE class USceneComponent* GetDummyRoot() const { return DummyRoot; }
	/** Returns ScoreText subobject **/
	FORCEINLINE class UTextRenderComponent* GetScoreText() const { return ScoreText; }

	bool IsInGameOver() const;
	void SetGameOverState();

private:
	bool IsInGameOverState;
	// Sequence block grids value.
	int SecuenceBlockValue[SIZE_SECUENCE] = {
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
		0, 0, 0, 0, 0, 0, 0, 0,
	};
	
	int MinesPositions[NUMBER_MINES] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

	void CreateRandomMinesPositions();
	void CreateSequenceBlockValues();
	bool IsMinePosition(int Position);
	bool IsAMinePosition(int Position);
	int NumberAMinePosition(int Position);

	int NumberRow(int IndexBlock);
	int NumberCol(int IndexBlock);
	int CalcIndexBlock(int Row, int Col);

};
