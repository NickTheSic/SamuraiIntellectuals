// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyBase.h"

#include "../Waypoints/Waypoint.h"
#include "../Waypoints/WaypointGroup.h"
#include "../Waypoints/WaypointManager.h"
#include "AIModule/Classes/Blueprint/AIBlueprintHelperLibrary.h"
#include "Engine.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SkeletalMeshComponent.h"

#define debugprint(string, color); bDoesDebugDrawOnScreenMessages ? GEngine->AddOnScreenDebugMessage(-1, 2, color, string) : bDoesDebugDrawOnScreenMessages = 0;

// Sets default values
AEnemyBase::AEnemyBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//It won;t send the messages for the Enemy in the build
#if WITH_EDITOR
	bDoesDebugDrawOnScreenMessages = true;
#else
	bDoesDebugDrawOnScreenMessages = false;
#endif

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
			SetWaypointManager(newManager); //The function has a check macro, might be good to have it a function anyway
			//TODO: Old debug code to remove - 
			//debugprint("Waypoint Manager Found", FColor::Red);
		}

	}
}

void AEnemyBase::GetNewWaypoint()
{
	//TODO: Networking - GetLocalRole() == ROLE_Authority ??
	check(m_WaypointManager && "Waypoint manager was null");

	if (m_TargetWaypoint != nullptr)
	{
		//Set the old waypoint to Not Taken
		debugprint("Setting old waypoint to false", FColor::Magenta);
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

			GetNextGroup();
		}
		else
		{
			m_TargetWaypoint = m_WaypointManager->GetRandomWaypoint(m_CurrentWaypointGroup);

			//TODO: Nick says - I don't like this but I think it will work for now
			GetNextGroup();
		}

		if (m_TargetWaypoint != nullptr)
		{
			m_TargetWaypoint->SetIsWaypointTaken(true);

			UAIBlueprintHelperLibrary::SimpleMoveToActor(GetController(), m_TargetWaypoint);
		}
	}

	if (m_TargetWaypoint == nullptr)
	{
		//If we didn't find a point in the next group we fall back
		GetPreviousGroup();
	}

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
			debugprint("Within distance! Changing", FColor::Green);
			GetNewWaypoint();
		}

	}
	else
	{
		GetNewWaypoint(); //If we don't have a waypoint we want one.  There is a check within GetNewWaypoint
	}

}

void AEnemyBase::GetNextGroup()
{
#if WITH_EDITOR
	FString out;
	out = GetName();
	out.Append(TEXT(" is is going to next group"));
	debugprint(out, FColor::Red);
#endif

	m_CurrentWaypointGroup++;

	if (m_CurrentWaypointGroup >= m_WaypointManager->GetWaypointGroupSize())
	{
		//Reset the group int to 0
		m_CurrentWaypointGroup = 0;
	}
}

void AEnemyBase::GetPreviousGroup()
{

#if WITH_EDITOR
	FString out;
	out = GetName();
	out.Append(TEXT(" is falling back to previous group"));
	debugprint(out, FColor::Red);
#endif //Debuging stuff in here

	m_CurrentWaypointGroup--;

	if (m_CurrentWaypointGroup < 0)
	{
		//Reset the group int to Max-1
		m_CurrentWaypointGroup = m_WaypointManager->GetWaypointGroupSize() - 1;
	}
}