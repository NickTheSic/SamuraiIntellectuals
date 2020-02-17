// Copyright 2020 Samurai Intellectual - Coded by Nick Lemmon

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "..//Items/ItemBase.h"

#include "TowerShopMenu.generated.h"

/**
 * Change log:			
 *			2/12/2020: Nick - Changed up how the shop gets made.  Created helper functions to help me too		
	
			2/9/2020: Nick - Added an Exit menu button and added the dynamic delegate to call the exit menu

 *			2/8/2020:	Found out that there are functions in here that I am making (SetPlayerController)
 *						I might find out how to use them specifically or continue using mine
*
 *			2/3/2020: Initial creation of this file
 */

UCLASS()
class SI_JUSTSURVIVE_API UTowerShopMenu : public UUserWidget
{
	GENERATED_BODY()
	
public:

	void NativeConstruct() override;
	void NativeDestruct() override;

	void SetOwningPlayerController(class ASI_PlayerController* pc); //TODO: A Function exists like this, look into it but will use this until then

	void SetDisplayText(FShopData& data);

	void ClearDisplayText();

	UFUNCTION()
		void ExitMenu();

	void UpdateShopList();

protected:
	class ASI_PlayerController* m_OwningPC; //TODO: Add proper player controller when the time comes

	void InitializeShopList();

	//The purchaseable towers
	UPROPERTY(EditAnywhere, Category = "Config")
		TArray<TSubclassOf<class AItemBase>> m_ItemsToDisplay;

	UPROPERTY(EditAnywhere, Category = "Config")
		TSubclassOf<class UTowerShopButton> ButtonTemplate;

	//The following components need to be created with the same type and name inside the hud blueprint
	//TODO: Might need to add more to this list at a later date
	UPROPERTY(meta = (BindWidget))
		class UScrollBox* m_TowerList = nullptr;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* m_NameText = nullptr;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* m_CostText = nullptr;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* m_PlayerMoney = nullptr;

	UPROPERTY(meta = (BindWidget))
		class UButton* m_ExitButton = nullptr;

};
