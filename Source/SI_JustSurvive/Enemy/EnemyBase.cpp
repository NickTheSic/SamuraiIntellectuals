// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyBase.h"

#include "../Waypoints/Waypoint.h"
#include "../Waypoints/WaypointGroup.h"
#include "../Waypoints/WaypointManager.h"
#include "AIModule/Classes/Blueprint/AIBlueprintHelperLibrary.h"
#include "Engine.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SkeletalMeshComponent.h"

// Sets default values
AEnemyBase::AEnemyBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	GetCapsuleComponent()->SetNotifyRigidBodyCollision(true); 
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::NoCollision); 
	//TODO(Anyone): Networking - Wait it is a Character, are they networked?
	//SetReplicates(true)
	//SetReplicatesMovement(true);
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
	if (m_WaypointManager == nullptr)
	{
		TArray<AActor*> singleWaypointManager;

		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AWaypointManager::StaticClass(), singleWaypointManager);

		check(singleWaypointManager.Num() == 1 && "There was either 0 or 2+ Waypoint managers in this scene");

		AWaypointManager* newManager = Cast<AWaypointManager>(singleWaypointManager[0]);

		check(newManager && "New manager ended up being null, this probably shouldn't happen");

		if (newManager)
		{
			SetWaypointManager(newManager); 
		}

	}
}

void AEnemyBase::GetNewWaypoint()
{
	//TODO: Networking - GetLocalRole() == ROLE_Authority ??
	check(m_WaypointManager && "Waypoint manager was null");

	//check(FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld()));

	if (m_TargetWaypoint != nullptr)
	{
		//Set the old waypoint to Not Taken
		m_TargetWaypoint->SetIsWaypointTaken(false);
	}

	//TODO(Anyone): This will work for looping through.  But when we get to the generator it will need to be refactored
	if (m_WaypointManager)
	{
		check(m_WaypointManager->GetWaypointGroupSize() != 0);

		if (m_TargetWaypoint == nullptr || m_CurrentWaypointGroup >= m_WaypointManager->GetWaypointGroupSize())
		{
			m_CurrentWaypointGroup = 0;
			m_TargetWaypoint = m_WaypointManager->GetRandomWaypoint(m_CurrentWaypointGroup);
		}
		else
		{
			m_TargetWaypoint = m_WaypointManager->GetRandomWaypoint(m_CurrentWaypointGroup);
		}

		if (m_TargetWaypoint != nullptr)
		{
			m_TargetWaypoint->SetIsWaypointTaken(true);
			IncrementCurrentWaypointGroup();
			UAIBlueprintHelperLibrary::SimpleMoveToActor(GetController(), m_TargetWaypoint);
		}
	}

	if (m_TargetWaypoint == nullptr)
	{
		//If we didn't find a point in the next group we fall back
		DecrementCurrentWaypointGroup();
	}

}

// Called every frame
void AEnemyBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (m_WaypointManager == nullptr)
	{
		FindWaypointManager();
	}

	if (m_TargetWaypoint != nullptr)
	{
		FVector DistanceVector = GetActorLocation() - m_TargetWaypoint->GetActorLocation();

		float DistanceSize = DistanceVector.Size();

		if (DistanceSize < EnemyData.m_DistanceToPoint)
		{
			//TODO: Make this change states instead of getting a new Waypoint
			GetNewWaypoint();
		}

	}
	else
	{
		GetNewWaypoint(); //If we don't have a waypoint we want one.  There is a check within GetNewWaypoint
	}

}

void AEnemyBase::KillEnemy()
{
	this->Destroy(); 
}

void AEnemyBase::IncrementCurrentWaypointGroup()
{
	m_CurrentWaypointGroup++;

	if (m_CurrentWaypointGroup >= m_WaypointManager->GetWaypointGroupSize())
	{
		//Reset the group int to 0
		m_CurrentWaypointGroup = 0;
	}
}

void AEnemyBase::DecrementCurrentWaypointGroup()
{
	m_CurrentWaypointGroup--;

	if (m_CurrentWaypointGroup < 0)
	{
		//Reset the group int to Max-1
		m_CurrentWaypointGroup = m_WaypointManager->GetWaypointGroupSize() - 1;
	}
}