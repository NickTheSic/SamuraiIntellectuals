// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "GameFrwkSessionsPlugin/GameFrwkSessionsPlugin.h"
#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "OnlineSubsystem.h"
#include "Interfaces/OnlineSessionInterface.h"

#include "MenuSystem/MenuInterface.h"
#include "OnlineGameInstance.generated.h"

USTRUCT()
struct FMapData
{
	GENERATED_BODY()

	UPROPERTY()
	FString MapName;

	UPROPERTY()
	FString MapPath;

};

/**
 * 
 */
UCLASS()
class GAMEFRWKSESSIONSPLUGIN_API UOnlineGameInstance : public UGameInstance, public IMenuInterface
{
	GENERATED_BODY()

public:
	UOnlineGameInstance(const FObjectInitializer & ObjectInitializer);

	virtual void Init() override;

	UFUNCTION(BlueprintCallable)
	void LoadMenuWidget();

	UFUNCTION(BlueprintCallable)
	void InGameLoadMenu();

	UFUNCTION(Exec)
	virtual void Host(FString ServerName, FString Team, FString Map) override;

	UFUNCTION(Exec)
	virtual void Join(uint32 Index, FString Team) override;

	void StartSession();

	virtual void LoadMainMenu() override;

	void RefreshServerList() override;
    
    FString DesiredTeamNum;

	FString MapToPlay;

	FORCEINLINE TArray<TSharedPtr<FMapData>> GetMapList(){ return MapList;} 

	void SetMapToPlay(FString map);

private:
	TSubclassOf<class UUserWidget> MenuClass;
	TSubclassOf<class UUserWidget> InGameMenuClass;

	class UMainMenu* Menu;

	IOnlineSessionPtr SessionInterface;
	TSharedPtr<class FOnlineSessionSearch> SessionSearch;

   TArray<TSharedPtr<FMapData>> MapList;

	void OnCreateSessionComplete(FName SessionName, bool Success);
	void OnDestroySessionComplete(FName SessionName, bool Success);
	void OnFindSessionsComplete(bool Success);
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);

	FString DesiredServerName;
   
	void CreateSession();

	void ParseMaps();
};
