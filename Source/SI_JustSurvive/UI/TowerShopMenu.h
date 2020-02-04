// Copyright 2020 Samurai Intellectual - Coded by Nick Lemmon

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"

#include "..//Items/ItemBase.h"

#include "TowerShopMenu.generated.h"

/**
 * Change log:
 *			TODO: Check about using a CameraActor instead of making your own.  Might not work since it is an actor not a pawn
 *			TODO:  Check over TODOs that can be fixed after a merge
 *			2/3/2020: Initial creation of this file
 */
UCLASS()
class SI_JUSTSURVIVE_API UTowerShopMenu : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void NativeConstruct() override;
	void NativeDestruct() override;

	void SetOwningPlayerController(class APlayerController* pc); //TODO: Check if something like this function exists

	void SetDisplayText(FShopData& data);

protected:
	class APlayerController* m_OwningPC; //TODO: Add proper player controller when the time comes

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

};
