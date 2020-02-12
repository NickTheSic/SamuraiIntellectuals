// Copyright 2020 Samurai Intellectuals


#include "WaypointManager.h"
#include "Waypoint.h"
#include "WaypointGroup.h"

#include "Engine.h"

#define debugprint(string); GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Blue, string)

// Sets default values
AWaypointManager::AWaypointManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

AWaypoint* AWaypointManager::GetRandomWaypoint( int group)
{
	check(m_WaypointGroups.Num() != 0 && "The waypoints number was empty");
	check(group >= 0 && group < m_WaypointGroups.Num() && "This group is out of range");
	

	if (group >= 0 && group < m_WaypointGroups.Num())
	{
		return m_WaypointGroups[group]->GetRandomWaypoint();
	}

	return nullptr; //Will always need to check for nullptr when getting a random waypoint
}

int AWaypointManager::GetWaypointGroupSize()
{
	return m_WaypointGroups.Num();
}

// Called when the game starts or when spawned
void AWaypointManager::BeginPlay()
{
	Super::BeginPlay();
	
}


