// Fill out your copyright notice in the Description page of Project Settings.

#include "LobbyGameMode.h"

#include "TimerManager.h"

#include "OnlineGameInstance.h"

void ALobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	++NumberOfPlayers;

	if (NumberOfPlayers >= MaxNumberOfPlayers)
	{
		GetWorldTimerManager().SetTimer(GameStartTimer, this, &ALobbyGameMode::StartGame, 10);
	}

	
}

void ALobbyGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);
	--NumberOfPlayers;
}

void ALobbyGameMode::StartGame()
{
	auto GameInstance = Cast<UOnlineGameInstance>(GetGameInstance());

	if (GameInstance == nullptr) return;

	//GameInstance->MapToPlay = "TestMap";
	
	GameInstance->StartSession();

	UWorld* World = GetWorld();
	if (!ensure(World != nullptr)) return;
	//World'/GameFrwkSessionsPlugin/Maps/Game.Game'  --- World'/Game/Levels/Sandbox/LV_Sandbox.LV_Sandbox'
	bUseSeamlessTravel = true;
	//World->ServerTravel("/Game/Levels/Sandbox/LV_Sandbox?listen");

    //World->ServerTravel("/Game/Levels/Sandbox/TestMap?listen");
	//World->ServerTravel("/Game/Levels/Sandbox/" + GameInstance->MapToPlay + "?listen");
	World->ServerTravel(GameInstance->MapToPlay + "?listen");
    //World->SeamlessTravel("/Game/Levels/Sandbox/TestMap?listen", true);
}
