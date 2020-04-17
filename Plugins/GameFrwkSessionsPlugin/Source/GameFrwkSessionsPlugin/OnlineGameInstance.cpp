// Fill out your copyright notice in the Description page of Project Settings.

#include "OnlineGameInstance.h"

#include "Engine/Engine.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"
#include "OnlineSessionSettings.h"
#include "JsonUtilities.h"

#include "MenuSystem/MainMenu.h"
#include "MenuSystem/MenuWidget.h"

const static FName SESSION_NAME = TEXT("Game");
const static FName SERVER_NAME_SETTINGS_KEY = TEXT("ServerName");

UOnlineGameInstance::UOnlineGameInstance(const FObjectInitializer & ObjectInitializer)
{
	//WidgetBlueprint'/GameFrwkSessionsPlugin/MenuSystem/WBP_InGameMenu.WBP_InGameMenu'
	ConstructorHelpers::FClassFinder<UUserWidget> MenuBPClass(TEXT("/GameFrwkSessionsPlugin/MenuSystem/WBP_MainMenu"));
	if (!ensure(MenuBPClass.Class != nullptr)) return;

	MenuClass = MenuBPClass.Class;

	ConstructorHelpers::FClassFinder<UUserWidget> InGameMenuBPClass(TEXT("/GameFrwkSessionsPlugin/MenuSystem/WBP_InGameMenu"));
	if (!ensure(InGameMenuBPClass.Class != nullptr)) return;

	InGameMenuClass = InGameMenuBPClass.Class;
}

void UOnlineGameInstance::Init()
{   
	Super::Init();

	IOnlineSubsystem* Subsystem = IOnlineSubsystem::Get();
	if (Subsystem != nullptr) {
		UE_LOG(LogTemp, Warning, TEXT("Found subsystem %s"), *Subsystem->GetSubsystemName().ToString());
		SessionInterface = Subsystem->GetSessionInterface();
		if (SessionInterface.IsValid()) {
			SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UOnlineGameInstance::OnCreateSessionComplete);
			SessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this, &UOnlineGameInstance::OnCreateSessionComplete);
			SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UOnlineGameInstance::OnFindSessionsComplete);
			SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &UOnlineGameInstance::OnJoinSessionComplete);
		}
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Found no subsystem"));
	}
	
	ParseMaps();

}

void UOnlineGameInstance::LoadMenuWidget()
{
	if (!ensure(MenuClass != nullptr)) return;

	Menu = CreateWidget<UMainMenu>(this, MenuClass);
	if (!ensure(Menu != nullptr)) return;

	Menu->Setup();

	Menu->SetMenuInterface(this);
}

void UOnlineGameInstance::InGameLoadMenu()
{
	if (!ensure(InGameMenuClass != nullptr)) return;

	UMenuWidget* pMenu = CreateWidget<UMenuWidget>(this, InGameMenuClass);
	if (!ensure(pMenu != nullptr)) return;

	pMenu->Setup();

	pMenu->SetMenuInterface(this);
}


void UOnlineGameInstance::Host(FString ServerName, FString Team, FString Map)
{
	DesiredServerName = ServerName;
    DesiredTeamNum = Team;
	//MapToPlay = Map;
	if (SessionInterface.IsValid())
	{
		auto ExistingSession = SessionInterface->GetNamedSession(SESSION_NAME);
		if (ExistingSession != nullptr) 
		{
			SessionInterface->DestroySession(SESSION_NAME);
		}
		else
		{
			CreateSession();
		}
	}
}

void UOnlineGameInstance::OnDestroySessionComplete(FName SessionName, bool Success)
{
	if (Success) {
		CreateSession();
	}
}

void UOnlineGameInstance::CreateSession()
{
	if (SessionInterface.IsValid()) {
		FOnlineSessionSettings SessionSettings;
		FName name = IOnlineSubsystem::Get()->GetSubsystemName();
		if (name.IsEqual("NULL"))
		{
			SessionSettings.bIsLANMatch = true;
		}
		else
		{
			SessionSettings.bIsLANMatch = false;
		}
		SessionSettings.NumPublicConnections = 5;
		SessionSettings.bShouldAdvertise = true;
		SessionSettings.bUsesPresence = true;
		SessionSettings.Set(SERVER_NAME_SETTINGS_KEY, DesiredServerName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);

		SessionInterface->CreateSession(0, SESSION_NAME, SessionSettings);
	}
}

void UOnlineGameInstance::ParseMaps()
{
	const FString JsonFilePath = FPaths::ProjectContentDir() + "/JsonFiles/maps.json";
	FString JsonString; //Json converted to FString

	FFileHelper::LoadFileToString(JsonString, *JsonFilePath);

	//Displaying the json in a string format inside the output log
	GLog->Log("Json String:");
	GLog->Log(JsonString);

	//Create a json object to store the information from the json string
	//The json reader is used to deserialize the json object later on
	TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());
	TSharedRef<TJsonReader<>> JsonReader = TJsonReaderFactory<>::Create(JsonString);


	if (FJsonSerializer::Deserialize(JsonReader, JsonObject) && JsonObject.IsValid())
	{
		//The person "object" that is retrieved from the given json file

		TSharedPtr<FJsonObject> MapObject = JsonObject->GetObjectField("GameMaps");
		
		//Retrieving an array property and printing each field
		TArray<TSharedPtr<FJsonValue>> objArray = MapObject->GetArrayField("maps");
		GLog->Log("printing maps names...");
		for (int32 index = 0; index < objArray.Num(); index++)
		{
		
			TSharedPtr<FJsonObject > map = objArray[index]->AsObject();
			//GLog->Log("name:" + objArray[index]->AsString());
			GLog->Log("name:" + map->GetStringField("name"));

			TSharedPtr <FMapData> MapData = MakeShared<FMapData>();
			MapData->MapName = map->GetStringField("name");

			TArray<TSharedPtr<FJsonValue>> mapsArray = map->GetArrayField("info");
			MapData->MapPath = mapsArray[0]->AsString();

			for (int32 index2 = 0; index2 < mapsArray.Num(); index2++)
			{
				GLog->Log("		info:" + mapsArray[index2]->AsString());
			}

			MapList.AddUnique(MapData);

		}
	}
	else
	{
		GLog->Log("couldn't deserialize");
	}

}

void UOnlineGameInstance::OnCreateSessionComplete(FName SessionName, bool Success)
{
	if (!Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("Could not create session"));
		return;
	}

	if (Menu != nullptr)
	{
		Menu->Teardown();
	}

	UEngine* Engine = GetEngine();
	if (!ensure(Engine != nullptr)) return;

	Engine->AddOnScreenDebugMessage(0, 2, FColor::Green, TEXT("Hosting"));

	UWorld* World = GetWorld();
	if (!ensure(World != nullptr)) return;
	//World'/GameFrwkSessionsPlugin/Maps/Lobby.Lobby'
	World->ServerTravel("/GameFrwkSessionsPlugin/Maps/Lobby?listen");
}

void UOnlineGameInstance::RefreshServerList()
{
	SessionSearch = MakeShareable(new FOnlineSessionSearch());
	if (SessionSearch.IsValid())
	{
		SessionSearch->bIsLanQuery = true;
		SessionSearch->MaxSearchResults = 100;
		SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);
		UE_LOG(LogTemp, Warning, TEXT("Starting Find Session"));
		SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
	}
}

void UOnlineGameInstance::SetMapToPlay(FString map)
{
	for (int i = 0; i < MapList.Num(); i++)
	{
		if(map == MapList[i]->MapName)
		{
			MapToPlay = MapList[i]->MapPath;
			break;
		}
	}
}

void UOnlineGameInstance::OnFindSessionsComplete(bool Success)
{
	if (Success && SessionSearch.IsValid() && Menu != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Finished Find Session"));

		TArray<FServerData> ServerNames;
		for (const FOnlineSessionSearchResult& SearchResult : SessionSearch->SearchResults)
		{
			UE_LOG(LogTemp, Warning, TEXT("Found session names: %s"), *SearchResult.GetSessionIdStr());
			FServerData Data;
			Data.MaxPlayers = SearchResult.Session.SessionSettings.NumPublicConnections;
			Data.CurrentPlayers = Data.MaxPlayers - SearchResult.Session.NumOpenPublicConnections;
			Data.HostUsername = SearchResult.Session.OwningUserName;
			FString ServerName;
			if (SearchResult.Session.SessionSettings.Get(SERVER_NAME_SETTINGS_KEY, ServerName))
			{
				Data.Name = ServerName;
			}
			else
			{
				Data.Name = "Could not find name.";
			}
			ServerNames.Add(Data);
		}

		Menu->SetServerList(ServerNames);
	}
}

void UOnlineGameInstance::Join(uint32 Index, FString Team)
{
    DesiredTeamNum = Team;
	if (!SessionInterface.IsValid()) return;
	if (!SessionSearch.IsValid()) return;

	if (Menu != nullptr)
	{
		Menu->Teardown();
	}

	SessionInterface->JoinSession(0, SESSION_NAME, SessionSearch->SearchResults[Index]);
}

void UOnlineGameInstance::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	if (!SessionInterface.IsValid()) return;

	FString Address;
	if (!SessionInterface->GetResolvedConnectString(SessionName, Address)) {
		UE_LOG(LogTemp, Warning, TEXT("Could not get connect string."));
		return;
	}

	UEngine* Engine = GetEngine();
	if (!ensure(Engine != nullptr)) return;

	Engine->AddOnScreenDebugMessage(0, 5, FColor::Green, FString::Printf(TEXT("Joining %s"), *Address));

	APlayerController* PlayerController = GetFirstLocalPlayerController();
	if (!ensure(PlayerController != nullptr)) return;
    
	PlayerController->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
}

void UOnlineGameInstance::StartSession()
{
	if (SessionInterface.IsValid())
	{
		SessionInterface->StartSession(SESSION_NAME);
	}
}

void UOnlineGameInstance::LoadMainMenu()
{
	APlayerController* PlayerController = GetFirstLocalPlayerController();
	if (!ensure(PlayerController != nullptr)) return;
	//World'/GameFrwkSessionsPlugin/MenuSystem/MainMenu.MainMenu'
	PlayerController->ClientTravel("/GameFrwkSessionsPlugin/MenuSystem/MainMenu", ETravelType::TRAVEL_Absolute);
}
