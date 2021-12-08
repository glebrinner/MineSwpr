// Fill out your copyright notice in the Description page of Project Settings.


#include "MineSWPRPawn.h"
#include "BlockActor.h"
#include "DrawDebugHelpers.h"
#include "Camera/CameraComponent.h"

// Sets default values
AMineSWPRPawn::AMineSWPRPawn() 
{
	AutoPossessPlayer = EAutoReceiveInput::Player0;
	GameInstance = Cast<UWebSocketGameInstance>(GetGameInstance());
}

void AMineSWPRPawn::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		
			if (UCameraComponent* OurCamera = PC->GetViewTarget()->FindComponentByClass<UCameraComponent>())
			{
				FVector Start = OurCamera->GetComponentLocation();
				FVector End = Start + (OurCamera->GetComponentRotation().Vector() * 8000.0f);
				TraceForBlock(Start, End, true);
			}
		
		else
		{
			FVector Start, Dir, End;
			PC->DeprojectMousePositionToWorld(Start, Dir);
			End = Start + (Dir * 8000.0f);
			TraceForBlock(Start, End, false);
		}
	}
}

void AMineSWPRPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("TriggerClick", IE_Pressed, this, &AMineSWPRPawn::TriggerClick);
	PlayerInputComponent->BindAction("Help", IE_Pressed, this, &AMineSWPRPawn::HelpServer);
	PlayerInputComponent->BindAction("ChooseGameLevel", IE_Pressed, this, &AMineSWPRPawn::NewLevelSession);
	PlayerInputComponent->BindAction("ReturnCurrentMap", IE_Pressed, this, &AMineSWPRPawn::CurrentMap);
}

void AMineSWPRPawn::CalcCamera(float DeltaTime, struct FMinimalViewInfo& OutResult)
{
	Super::CalcCamera(DeltaTime, OutResult);

	OutResult.Rotation = FRotator(-90.0f, -90.0f, 0.0f);
}


void AMineSWPRPawn::TriggerClick()
{
	if (CurrentBlockFocus)
	{
		CurrentBlockFocus->HandleClicked();
	}
}

void AMineSWPRPawn::TraceForBlock(const FVector& Start, const FVector& End, bool bDrawDebugHelpers)
{
	FHitResult HitResult;
	GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility);
	if (bDrawDebugHelpers)
	{
		DrawDebugLine(GetWorld(), Start, HitResult.Location, FColor::Red);
		DrawDebugSolidBox(GetWorld(), HitResult.Location, FVector(20.0f), FColor::Red);
	}
	if (HitResult.Actor.IsValid())
	{
		ABlockActor* HitBlock = Cast<ABlockActor>(HitResult.Actor.Get());
		if (CurrentBlockFocus != HitBlock)
		{
			if (CurrentBlockFocus)
			{
				CurrentBlockFocus->Highlight(false);
			}
			if (HitBlock)
			{
				HitBlock->Highlight(true);
			}
			CurrentBlockFocus = HitBlock;
		}
	}
	else if (CurrentBlockFocus)
	{
		CurrentBlockFocus->Highlight(false);
		CurrentBlockFocus = nullptr;
	}
}

void AMineSWPRPawn::HelpServer()
{
	if(GameInstance)
	{
		if(GameInstance->WebSocket->IsConnected())
		{
			GameInstance->WebSocket->Send("help");
		}
	}
}

void AMineSWPRPawn::NewLevelSession()
{
	if(GameInstance)
	{
		if(GameInstance->WebSocket->IsConnected())
		{
			GameInstance->WebSocket->Send("new <l>");
		}
	}
}

void AMineSWPRPawn::CurrentMap()
{
	if(GameInstance)
    	{
    		if(GameInstance->WebSocket->IsConnected())
    		{
    			GameInstance->WebSocket->Send("map");
    		}
    	}
}

void AMineSWPRPawn::OpenedCellCoord(FString CellX, FString CellY)
{
	if(GameInstance)
    	{
    		if(GameInstance->WebSocket->IsConnected())
    		{
    			GameInstance->WebSocket->Send("open <" + CellX + "> <" + CellY + ">");
    		}
    	}
}



