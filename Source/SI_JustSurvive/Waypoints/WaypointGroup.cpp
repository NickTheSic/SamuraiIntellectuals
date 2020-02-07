// Fill out your copyright notice in the Description page of Project Settings.


#include "WaypointGroup.h"
#include "Waypoint.h"

// Sets default values
AWaypointGroup::AWaypointGroup()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

AWaypoint* AWaypointGroup::GetRandomWaypoint()
{
	check(m_Waypoints.Num() != 0 && "No Waypoints were set"); //TODO: Verify which way the check statement works. Might have it backwards

	AWaypoint* testWaypoint = nullptr; //I only want to test it a few times for a new random waypoint
	unsigned int tries = 0;

	//TODO: Possibly rewrite this function
	while (testWaypoint == nullptr && tries < 3)
	{
		int randomPoint = FMath::RandRange(0, m_Waypoints.Num());
		if (m_Waypoints[randomPoint] != nullptr && m_Waypoints[randomPoint]->GetIsWaypointTaken() == false) //If the waypoint isn't taken or a nullptr we can use it
		{
			testWaypoint = m_Waypoints[randomPoint];
			break; //Break out of the while loop and go to return the testWaypoint
		}
		tries++;
	}

	return testWaypoint; //TODO: Look into a check about nullptrs and what should happen if all the next way points are filled up
}

// Called when the game starts or when spawned
void AWaypointGroup::BeginPlay()
{
	Super::BeginPlay();
	//TODO: Look into adding waypoints dynamically through Heirarchy or adding children
}

// Called every frame
void AWaypointGroup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

