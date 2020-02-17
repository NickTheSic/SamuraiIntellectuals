// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "ShopCameraPawn.generated.h"

/*
*Change Log:
*
*			2/9/2020: Nick - I made the camera a UPROPERTY so we edit in the editor

*			2/8/2020: Nick - Initial Creation.  Need to clean up the todos at a later time
*/

UCLASS()
class SI_JUSTSURVIVE_API AShopCameraPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AShopCameraPawn();

	void SetPlaceableObject(class ATowerBase* newTower);

	void OnClickPlaceObject();

	//TODO: Add a function that changes the mouse cursor colour if we can't place it
	bool  CheckCanPlaceUnderMouse();

	void EnteringShop();
	void ExitingShop();

	bool GetIsActiveInShop();
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Camera")
		class UCameraComponent* m_CameraComponent; //The camera

	class ATowerBase* m_PlaceableTower; //The current active object

	bool bIsActiveInShop = false;
	bool bCanPlaceTower = false;

	UPROPERTY(EditAnywhere, Category = "Config")
		float m_TrenchDepth = 0; //The depth of the trench.  We can cast to the top of the trench where towers can be placed but not in the trench

	//TODO: I want to add a texture that follows the mouse so that it can be changed.  Showing the player it can and cant be placed


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
