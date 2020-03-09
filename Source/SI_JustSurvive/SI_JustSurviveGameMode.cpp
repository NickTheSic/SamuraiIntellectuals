// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "SI_JustSurviveGameMode.h"
#include "SI_JustSurviveHUD.h"
#include "SI_JustSurviveCharacter.h"
#include "UObject/ConstructorHelpers.h"

ASI_JustSurviveGameMode::ASI_JustSurviveGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = ASI_JustSurviveHUD::StaticClass();
}
