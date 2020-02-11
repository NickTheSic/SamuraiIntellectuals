// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyBase.h"

#include "../Waypoints/Waypoint.h"
#include "../Waypoints/WaypointGroup.h"
#include "../Waypoints/WaypointManager.h"
#include "AIModule/Classes/Blueprint/AIBlueprintHelperLibrary.h"
#include "Engine.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SkeletalMeshComponent.h"

#define debugprint(string); GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Blue, string)

// Sets default values
AEnemyBase::AEnemyBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	
}

void AEnemyBase::SetWaypointManager(AWaypointManager* wayMan)
{
	check(wayMan && "Waypoint Manager was null");

	if (wayMan)
	{
		m_WaypointManager = wayMan;
	}
}

// Called when the game starts or when spawned
void AEnemyBase::BeginPlay()
{
	Super::BeginPlay();

	if (m_WaypointManager == nullptr)
	{
		FindWaypointManager();
	}

	if (EnemyData.m_EnemyMesh != nullptr)
	{
		GetMesh()->SetSkeletalMesh(EnemyData.m_EnemyMesh);
	}

}

void AEnemyBase::FindWaypointManager()
{
	//We only want to do this if the waypoint manager is null
	if (m_WaypointManager == nullptr) //Probably a redundant check
	{
		GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Blue, "Finding Waypoint Manager");

		TArray<AActor*> singleWaypointManager;

		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AWaypointManager::StaticClass(), singleWaypointManager);

		check(singleWaypointManager.Num() == 1 && "There was either 0 or 2+ Waypoint managers in this scene");

		AWaypointManager* newManager = Cast<AWaypointManager>(singleWaypointManager[0]); //Should only be one and it should be this type

		check(newManager && "New manager ended up being null, this probably shouldn't happen");

		if (newManager)
		{
			m_WaypointManager = newManager;
			debugprint("Waypoint Manager Found");
		}

	}
}

void AEnemyBase::GetNewWaypoint()
{

	check(m_WaypointManager && "Waypoint manager was null");

	if (m_WaypointManager)
	{
		check(m_WaypointManager->GetWaypointGroupSize() != 0);

		debugprint("Finding Waypoint");

		if (m_TargetWaypoint == nullptr || m_CurrentWaypointGroup >= m_WaypointManager->GetWaypointGroupSize())
		{
			debugprint("Targetpoint was null, waypoint group size was in range or somethin");
			m_CurrentWaypointGroup = 0;
			m_TargetWaypoint = m_WaypointManager->GetRandomWaypoint(m_CurrentWaypointGroup);
		}
		else
		{
			debugprint("Looping through other waypoints");
			m_TargetWaypoint = m_WaypointManager->GetRandomWaypoint(m_CurrentWaypointGroup);
		}

		UAIBlueprintHelperLibrary::SimpleMoveToActor(GetController(), m_TargetWaypoint);
	}

	//TODO: Nick says - I don't like this but I think it will work for now
	m_CurrentWaypointGroup++;

}

// Called every frame
void AEnemyBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (m_TargetWaypoint != nullptr)
	{
		FVector DistanceVector = GetActorLocation() - m_TargetWaypoint->GetActorLocation();

		float DistanceSize = DistanceVector.Size();

		if (DistanceSize < EnemyData.m_DistanceToPoint)
		{
			//TODO: Make this change states instead of getting a new Waypoint
			debugprint("Within distance! Changing");
			GetNewWaypoint();
		}

	}
	else
	{
		GetNewWaypoint(); //If we don't have a waypoint we want one.  There is a check within GetNewWaypoint
	}

}

