// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "GridActor.h"
#include "WebSocketGameInstance.h"
#include "MineSWPRPawn.generated.h"

UCLASS()
class MINESWPR_API AMineSWPRPawn : public APawn
{
	GENERATED_BODY()

	virtual void Tick(float DeltaSeconds) override;

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	virtual void CalcCamera(float DeltaTime, struct FMinimalViewInfo& OutResult) override;

	UPROPERTY()
	UWebSocketGameInstance* GameInstance;

public:
	
	AMineSWPRPawn();

	virtual void BeginPlay() override;

protected:
	void TriggerClick();
	void TraceForBlock(const FVector& Start, const FVector& End, bool bDrawDebugHelpers);

	void HelpServer();
	void NewLevelSession();
	void CurrentMap();
	void OpenedCellCoord(FString CellX, FString CellY);

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
	class ABlockActor* CurrentBlockFocus;
};
