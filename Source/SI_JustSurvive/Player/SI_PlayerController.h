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
	UPROPERTY(Replicated)
	class ASI_JustSurviveCharacter* MyOwningCharacter = nullptr; 

	//The tower menu for the player
    UPROPERTY()
	class UTowerShopMenu* MyTowerHud = nullptr; 

	UPROPERTY(EditAnywhere, Category = "Config", meta = (AllowPrivateAccess))
		TSubclassOf<UTowerShopMenu> TSHudTemplate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
		TSubclassOf<class UUserWidget> w_GameOverMenu;

	UPROPERTY()
	UUserWidget* GameOverMenu;


public:
	ASI_PlayerController();

	//The base functions that we need
	void OnPossess(class APawn* aPawn) override;
	void OnUnPossess() override;

	void SetupInputComponent() override;


	//I'll make this public for now
	UFUNCTION(Server, Reliable)
		void ServerCallEnterShop(AShopCameraPawn* camPawn);

	void EnterTowerShopMenu(); //Called on button press

	UFUNCTION(Server, Reliable)
		void ServerExitShop(AShopCameraPawn* camPawn);

	UFUNCTION( BlueprintCallable, Category = "Gameplay")
		void ExitTowerShopMenu();

	class UTowerShopMenu* GetTowerShopMenu() { return MyTowerHud; };

	UFUNCTION(BlueprintCallable, Category = "GameOver")
		void InitiateGameOver();

	//Functions to call on the pawn
	void Jump();
	void StopJumping();

	void StartInteraction();
	void StopInteraction();

	void MoveForward(float val);
	void MoveRight(float val);

	void OnMouseClick();
	void OnClickReleased(); 
	void Reload(); 
	void ChangeWeapon(float val);

	//Controller functions for the menu
	void ControllerXForMouseX(float val);
	void ControllerYForMouseY(float val);
	void ControllerForMouseClick();

	void ControllerForMouseUp();

protected:

	void BeginPlay() override;
};
