// Copyright 2020 Samurai Intellectuals

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "SI_PlayerState.generated.h"

/**
 * 
 */
UCLASS()
class SI_JUSTSURVIVE_API ASI_PlayerState : public APlayerState
{
	GENERATED_BODY()
	
		//TODO:  Re check how this is supposed to work

public:

	//Should some of this stuff go into the PlayerController instead?
	int m_MaxHealth;
	int m_CurrentHealth;

	int m_CurrentMoney;

	bool bPlayerIsInMenu;

};
