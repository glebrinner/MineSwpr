// Fill out your copyright notice in the Description page of Project Settings.


#include "WebSocketGameInstance.h"
#include "WebSocketsModule.h"

void UWebSocketGameInstance::Init()
{
	Super::Init();

	if(!FModuleManager::Get().IsModuleLoaded("WebSocket"))
	{
		FModuleManager::Get().LoadModule("WebSocket");
	}

	WebSocket = FWebSocketsModule::Get().CreateWebSocket(ServerAddress);
	
	WebSocket->OnConnected().AddLambda([]()
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, "Connected");
	});
	
	WebSocket->OnConnectionError().AddLambda([](const FString& Error)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::Red, Error);
	});

	WebSocket->OnClosed().AddLambda([](int StatusCode, const FString& Reason, bool IsWasClean)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.f, IsWasClean ? FColor::Green : FColor::Red, "Connection is closed" + Reason);
	});

	WebSocket->OnMessage().AddLambda([](const FString& Message)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::White, "Message from Server :" + Message);
	});

	WebSocket->OnMessageSent().AddLambda([](const FString& Message)
	{
		GEngine->AddOnScreenDebugMessage(-1, 15.f, FColor::White, "Message TO Server :" + Message);
	});
	
	WebSocket->Connect();
	
}

void UWebSocketGameInstance::Shutdown()
{
	Super::Shutdown();

	if(WebSocket->IsConnected())
	{
		WebSocket->Close();
	}
}

