// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "SI_PlayerController.generated.h"

/**
 * 
 */
UCLASS()
class SI_JUSTSURVIVE_API ASI_PlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	
	//A Reference to the initial pawn we possess so that we can repossess it
	class ASI_JustSurviveCharacter* MyOwningCharacter = nullptr; 

	//The tower menu for the player
	class UTowerShopMenu* MyTowerHud; 

	UPROPERTY(EditAnywhere, Category = "Config", meta = (AllowPrivateAccess))
		TSubclassOf<UTowerShopMenu> TSHudTemplate;

public:

	//The base functions that we need
	void OnPossess(class APawn* aPawn) override;
	void OnUnPossess() override;

	void SetupInputComponent() override;


	//I'll make this public for now
	void EnterTowerShopMenu(); //Called on button press

	UFUNCTION(BlueprintCallable, Category = "Gameplay")
		void ExitTowerShopMenu();

	class UTowerShopMenu* GetTowerShopMenu() { return MyTowerHud; };


	//Functions to call on the pawn
	void Jump();
	void StopJumping();

	void StartInteraction();
	void StopInteraction();

	void MoveForward(float val);
	void MoveRight(float val);

	void OnMouseClick(); //Will call fire on PlayerCharacter, but will do something else in the shop menu
	void OnClickReleased(); 
	void Reload(); 
	void ChangeWeapon(float val);

protected:

	void BeginPlay() override;
};
