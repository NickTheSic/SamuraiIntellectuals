// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "ShopCameraPawn.generated.h"

/*
*Change Log:
*
*			2/8/2020: Nick - Initial Creation.  Need to clean up the todos at a later time
*/

UCLASS()
class SI_JUSTSURVIVE_API AShopCameraPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AShopCameraPawn();

	void SetPlaceableObject(class AItemBase* newTower);

	void OnClickPlaceObject();

	//TODO: Add a function that changes the mouse cursor colour if we can't place it
	bool  CheckCanPlaceUnderMouse();

	void EnteringShop();
	void ExitingShop();
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	class UCameraComponent* m_CameraComponent; //The camera

	class AItemBase* m_PlaceableTower; //The current active object

	bool bIsActiveInShop = false;


	//TODO: I want to add a texture that follows the mouse so that it can be changed.  Showing the player it can and cant be placed

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
