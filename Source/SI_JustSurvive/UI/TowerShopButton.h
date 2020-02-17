// Copyright 2020 Samurai Intellectual - Coded by Nick Lemmon

#pragma once

#include "CoreMinimal.h"
#include "Components/Button.h"
#include "TowerShopButton.generated.h"

/**
 * Change Log:
*			2/12/2020: Nick - I made minor changes to the comments and format

 *			2/9/2020: Nick - I added a null texture check when setting the button texture among other things to the widget style

 *			2/3/2020: Initial creation of this file
 */
UCLASS()
class SI_JUSTSURVIVE_API UTowerShopButton : public UButton
{
	GENERATED_BODY()
	
public:
	UTowerShopButton();

	UFUNCTION()
		void OnButtonClick();

	void GiveSpawnItemToPlayer();

	void SetObjectToCreate(class AItemBase* newItem);

	void SetupWidgetStyle();
	void SetDisplayData();

	void SetOwningHud(class UTowerShopMenu* menu); //TODO: Should we check for a function that already does this

	int GetTowerCost();

protected:
	class UTowerShopMenu* m_TowerHud = nullptr;

	class ATowerBase* m_SpawnItem = nullptr; //TODO: Check if this could be AItemBase or specifically a Tower

};
