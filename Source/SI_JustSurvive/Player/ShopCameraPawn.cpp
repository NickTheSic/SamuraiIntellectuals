// Fill out your copyright notice in the Description page of Project Settings.


#include "ShopCameraPawn.h"
#include "Camera/CameraComponent.h"
#include "SI_PlayerController.h"
#include "../Items/ItemBase.h"
#include "../Items/TowerBase.h"
#include "../UI/TowerShopMenu.h"
#include "Engine/Engine.h"
#include "DrawDebugHelpers.h"
#include "Net/UnrealNetwork.h"

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

	SetReplicates(true);
	SetReplicateMovement(true);
}

void AShopCameraPawn::SetPlaceableObject(ATowerBase * newTower)
{
	check(newTower && "The tower passed in was Nullptr"); //TODO: Make sure this works properly
	m_PlaceableTower = newTower;
}

void AShopCameraPawn::OnClickPlaceObject()
{
	//if (m_PlaceableTower != nullptr && bCanPlaceTower)
	if (CheckCanPlaceUnderMouse() && m_PlaceableTower != nullptr)
	{

		UWorld* world = GetWorld();
		ASI_PlayerController* pc = Cast<ASI_PlayerController>(GetController());

		check(world && pc && "One of these returned a nullptr and I am not sure why we would have this issue"); //Nick wrote this
		
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

			//Pass it to the playercontroller since it is on the server
			pc->PlaceTower(this, tower, transform, m_PlaceableTower);

			//Update the Menu with the new money we have and deactivate the buttons if we don't have enough
			pc->GetTowerShopMenu()->UpdateShopList();
		}
	}
}

void AShopCameraPawn::ServerPlaceObject(TSubclassOf<ATowerBase> tower, FTransform transform, ATowerBase* placeableTower)
{
	if (GetLocalRole() == ROLE_Authority)
	{
		FActorSpawnParameters spawnParams;

		ATowerBase* ref = GetWorld()->SpawnActor<ATowerBase>(tower, transform, spawnParams);

		//I have to set the data to the actual towers data
		ref->SetShopData(placeableTower->GetShopData());
		ref->SetIsInShop(false); //Now that it is puchased and in the world it isn't in the shop
		ref->m_TowerData = placeableTower->m_TowerData;
		ref->InitializeTower();
	}
}

bool AShopCameraPawn::CheckCanPlaceUnderMouse()
{
		UWorld* world = GetWorld();
		ASI_PlayerController* pc = Cast<ASI_PlayerController>(GetController());

		//The trench depth should be set
		check(m_TrenchDepth != 0.0f && "The trench depth might not've been set, will not work");

		if (world && pc && pc->IsLocalPlayerController())
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

			//DrawDebugLine(world, Start, End, FColor::Cyan, true);

			return world->LineTraceSingleByObjectType(hit, Start, End, FCollisionObjectQueryParams::AllObjects, FCQP);
		}
	
	return false;
}

bool AShopCameraPawn::EnteringShop_Validate()
{
	return true;
}

bool AShopCameraPawn::ExitingShop_Validate()
{
	return true;
}

void AShopCameraPawn::EnteringShop_Implementation()
{
	//TODO: Setup all the defaults for entering a shop
	if (GetLocalRole() == ROLE_Authority)
	{
		bIsActiveInShop = true;
		m_PlaceableTower = nullptr;
		bCanPlaceTower = false;
	}
}

void AShopCameraPawn::MoveUp(float val)
{
	if (val != 0)
	{
		FVector pos = GetActorLocation();
		pos.X += val * 50;
		SetActorLocation(pos);
	}
}

void AShopCameraPawn::MoveRight(float val)
{
	if (val != 0)
	{
		FVector pos = GetActorLocation();
		pos.Y += val * 50;
		SetActorLocation(pos);
	}
}

void AShopCameraPawn::Zoom(float val)
{

}

void AShopCameraPawn::ExitingShop_Implementation()
{
	if (GetLocalRole() == ROLE_Authority)
	{
		//TODO: Setup all defaults for exiting the shop
		bIsActiveInShop = false;
		m_PlaceableTower = nullptr;
		bCanPlaceTower = false;
	}
}

bool AShopCameraPawn::GetIsActiveInShop()
{
	return bIsActiveInShop;
}

// Called when the game starts or when spawned
void AShopCameraPawn::BeginPlay()
{
	Super::BeginPlay();
	
	//Check them here just to be sure
	//SetReplicates(true);
	//SetReplicateMovement(true);
}

// Called every frame
void AShopCameraPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AShopCameraPawn::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AShopCameraPawn, m_PlaceableTower);
	DOREPLIFETIME(AShopCameraPawn, bIsActiveInShop);
	DOREPLIFETIME(AShopCameraPawn, m_TrenchDepth);
}
