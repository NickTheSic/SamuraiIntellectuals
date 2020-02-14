// Copyright 2020 Samurai Intellectuals

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "SI_PlayerState.generated.h"

/**
 * Change Log:
		2/12/2020: Nick - I added functions to get and set values
						- Looked into some ideas for functions
	
		Initial Creation
 */
UCLASS()
class SI_JUSTSURVIVE_API ASI_PlayerState : public APlayerState
{
	GENERATED_BODY()
	
		//TODO:  Re check how this is supposed to work

public:

	//Should some of this stuff go into the PlayerController instead?
	//TODO:Should we have these functions Bluepirnt callable?
	//TODO: Probably going to have to network some of these functions
	UFUNCTION(BlueprintPure, Category = "ASI PlayerState")
		float GetMaxHealth();

	UFUNCTION(BlueprintPure, Category = "ASI PlayerState")
		float GetCurrentHealth();

	UFUNCTION(BlueprintPure, Category = "ASI Player State")
		int GetCurrentMoney();

	UFUNCTION(Exec)
		void AddCurrentMoney(int newMoneyIn);

	void SubtractCurrentMoney(int newMoneyPaid);

	bool GetPlayerIsInMenu();
	void SetPlayerIsInMenu(bool toggle);

	//TODO(Nick) void Get Player Health Below 20 Percent() 
	//return to notify other players this one needs help?

protected:
	//TODO: Do we need to replicate these variables
	//TODO:  Ask about doing the health with OnTakeDamage and FDamage
	float m_MaxHealth;

	//Replicated
	float m_CurrentHealth;
	//Replicated
	int m_CurrentMoney;
	//Replicate?
	bool bPlayerIsInMenu;

};
