// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "IWebSocket.h"
#include "WebSocketGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class MINESWPR_API UWebSocketGameInstance : public UGameInstance
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	FString ServerAddress = "wss://hometask.eg1236.com/game1/";
	FString LocalAddress = "ws://localhost:8080";
public:
	TSharedPtr<IWebSocket> WebSocket;

	virtual void Init() override;
	virtual  void Shutdown() override;
	
};
