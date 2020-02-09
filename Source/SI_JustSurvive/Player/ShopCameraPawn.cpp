// Fill out your copyright notice in the Description page of Project Settings.


#include "ShopCameraPawn.h"
#include "Camera/CameraComponent.h"
#include "../Items/ItemBase.h"
#include "../Items/TowerBase.h"

// Sets default values
AShopCameraPawn::AShopCameraPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	m_CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComp");
	//TODO: Setup more defaults here

	//Raycasting works better in orthographic mode
	m_CameraComponent->SetProjectionMode(ECameraProjectionMode::Orthographic);

	bIsActiveInShop = false;
	m_PlaceableTower = nullptr;
}

void AShopCameraPawn::SetPlaceableObject(AItemBase * newTower)
{
	check(newTower && "The tower passed in was Nullptr"); //TODO: Make sure this works properly
	m_PlaceableTower = newTower;
}

void AShopCameraPawn::OnClickPlaceObject()
{
	if (m_PlaceableTower != nullptr)
	{
		/*TODO:
		//if (PlaceableTower->Purchase( Getplayer controller or player state ))
		//{
		//Cast a ray to place an instance of the PlaceableTower
		//
		//}
		*/
	}
}

bool AShopCameraPawn::CheckCanPlaceUnderMouse()
{
	//TODO:
	//Cast a ray under the mouse cursor
	//If the ray hits a collider that we can place on
	//
	return false;
}

void AShopCameraPawn::EnteringShop()
{
	//TODO:
	bIsActiveInShop = true;
	m_PlaceableTower = nullptr;

}

void AShopCameraPawn::ExitingShop()
{
	//TODO:
	bIsActiveInShop = false;
	m_PlaceableTower = nullptr;

}

// Called when the game starts or when spawned
void AShopCameraPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AShopCameraPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//I Only want to cast a ray if we are in the shop with a tower selected
	if (bIsActiveInShop == true && m_PlaceableTower != nullptr)
	{
		CheckCanPlaceUnderMouse();

	}

}

// Called to bind functionality to input
void AShopCameraPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	//TODO: Note, I don't think this is needed;
}

