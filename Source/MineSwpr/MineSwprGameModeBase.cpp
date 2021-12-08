// Copyright Epic Games, Inc. All Rights Reserved.


#include "MineSwprGameModeBase.h"

#include "MineSWPRPawn.h"
#include "MineSWPRPlayerController.h"

AMineSwprGameModeBase::AMineSwprGameModeBase()
{
	DefaultPawnClass = AMineSWPRPawn::StaticClass();
	PlayerControllerClass = AMineSWPRPlayerController::StaticClass();
}
