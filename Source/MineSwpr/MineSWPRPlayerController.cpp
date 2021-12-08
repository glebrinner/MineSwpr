// Fill out your copyright notice in the Description page of Project Settings.


#include "MineSWPRPlayerController.h"

AMineSWPRPlayerController::AMineSWPRPlayerController()
{
	bShowMouseCursor = true;
	bEnableClickEvents = true;
	bEnableTouchEvents = true;
	DefaultMouseCursor = EMouseCursor::Crosshairs;
	ClickEventKeys.Add(EKeys::RightMouseButton);
}
