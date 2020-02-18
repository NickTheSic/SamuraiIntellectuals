// Fill out your copyright notice in the Description page of Project Settings.


#include "ShopCameraPawn.h"
#include "Camera/CameraComponent.h"
#include "SI_PlayerController.h"
#include "../Items/ItemBase.h"
#include "../Items/TowerBase.h"
#include "../UI/TowerShopMenu.h"
#include "Engine/Engine.h"
#include "DrawDebugHelpers.h"

// Sets default values
AShopCameraPawn::AShopCameraPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	m_CameraComponent = CreateDefaultSubobject<UCameraComponent>("CameraComp");
	//TODO: Setup more defaults here
	//Raycasting works better in orthographic mode
	m_CameraComponent->SetProjectionMode(ECameraProjectionMode::Orthographic);
	m_CameraComponent->SetOrthoWidth(5000);

	bIsActiveInShop = false;
	m_PlaceableTower = nullptr;
}

void AShopCameraPawn::SetPlaceableObject(ATowerBase * newTower)
{
	check(newTower && "The tower passed in was Nullptr"); //TODO: Make sure this works properly
	m_PlaceableTower = newTower;
}

void AShopCameraPawn::OnClickPlaceObject()
{
	if (m_PlaceableTower != nullptr && bCanPlaceTower)
	{
		//TODO: OnCLickPLaceObject function
		//if (PlaceableTower->Purchase( Getplayer controller or player state ))
		//{
		//Cast a ray to place an instance of the PlaceableTower
		//
		//}

		UWorld* world = GetWorld();
		ASI_PlayerController* pc = Cast<ASI_PlayerController>(GetController());

		check(world && pc && "One of these returned a nullptr and I am not sure why we would have this issue"); //Nick wrote this

		//We check if we can purchase the item
		//If we can, then we can place it from here instead of within the TowerBase
		bool didPurchase = m_PlaceableTower->Purchase(pc);

		if (!didPurchase)
		{
			return;
		}

		if (world && pc && didPurchase)
		{
			TSubclassOf<ATowerBase> tower = Cast<ATowerBase>(m_PlaceableTower)->GetClass();

			FHitResult hit;
			float xLoc, yLoc;

			pc->GetMousePosition(xLoc, yLoc);
			FVector Start(xLoc, yLoc, GetActorLocation().Z);
			FVector Direction;
			pc->DeprojectMousePositionToWorld(Start, Direction);
			FVector End = Start;
			End.Z -= (GetActorLocation().Z - m_TrenchDepth);

			FCollisionQueryParams FCQP;
			FCQP.AddIgnoredActor(this);

			DrawDebugLine(world, Start, End, FColor::Cyan, true);

			world->LineTraceSingleByObjectType(hit, Start, End, FCollisionObjectQueryParams::AllObjects, FCQP);

			FTransform transform(hit.ImpactPoint);

			FActorSpawnParameters spawnParams;

			ATowerBase* ref = world->SpawnActor<ATowerBase>(tower, transform, spawnParams);

			//I have to set the data to the actual towers data
			ref->SetShopData(m_PlaceableTower->GetShopData());
			ref->SetIsInShop(false); //Now that it is puchased and in the world it isn't in the shop
			ref->m_TowerData = m_PlaceableTower->m_TowerData;
			ref->InitializeTower();

			//Update the Menu with the new money we have and deactivate the buttons if we don't have enough
			pc->GetTowerShopMenu()->UpdateShopList();
		}


	}
}

bool AShopCameraPawn::CheckCanPlaceUnderMouse()
{
	//TODO:  CheckCanPlaceMouse function
	//Cast a ray under the mouse cursor
	//If the ray hits a collider that we can place on

	UWorld* world = GetWorld();
	ASI_PlayerController* pc = Cast<ASI_PlayerController>(GetController());

	check(world && pc && "One of these returned a nullptr and I am not sure why we would have this issue"); //Nick wrote this
		
	if (world && pc)
	{

		FHitResult hit;
		float xLoc, yLoc;

		pc->GetMousePosition(xLoc, yLoc);
		FVector Start(xLoc, yLoc, GetActorLocation().Z);
		FVector Direction; 
		pc->DeprojectMousePositionToWorld(Start, Direction);
		FVector End = Start;
		End.Z -= (GetActorLocation().Z - m_TrenchDepth);

		FCollisionQueryParams FCQP;
		FCQP.AddIgnoredActor(this);

		DrawDebugLine(world, Start, End, FColor::Cyan, true);

		return world->LineTraceSingleByObjectType(hit, Start, End, FCollisionObjectQueryParams::AllObjects, FCQP);
	}

	return false;
}

void AShopCameraPawn::EnteringShop()
{
	//TODO: Setup all the defaults for entering a shop
	bIsActiveInShop = true;
	m_PlaceableTower = nullptr;
	bCanPlaceTower = false;
}

void AShopCameraPawn::ExitingShop()
{
	//TODO: Setup all defaults for exiting the shop
	bIsActiveInShop = false;
	m_PlaceableTower = nullptr;
	bCanPlaceTower = false;
}

bool AShopCameraPawn::GetIsActiveInShop()
{
	return bIsActiveInShop;
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
		bCanPlaceTower = CheckCanPlaceUnderMouse();
		if (bCanPlaceTower)
		{
			int b = 0;
		}
	}
}

