// Copyright 2020 Samurai Intellectuals


#include "WaypointGroup.h"
#include "Waypoint.h"
#include "Engine.h"

#define debugprint(string); GEngine->AddOnScreenDebugMessage(-1, 2, FColor::Blue, string)

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

	//TODO: Probably clean up my debug stuff - Nick

	debugprint("Entered the Get Random waypoint function");

	//TODO: Possibly rewrite this function.  This also looks messy
	while (testWaypoint == nullptr && tries < 3)
	{
		int randomPoint = FMath::RandRange(0, m_Waypoints.Num() - 1);

		check(randomPoint < m_Waypoints.Num() && "The random point was greater than or eqaul to the num"); //Logical issue on random

		if (m_Waypoints[randomPoint] != nullptr && m_Waypoints[randomPoint]->GetIsWaypointTaken() == false) //If the waypoint isn't taken or a nullptr we can use it
		{
			FString str = TEXT("Testing finding a waypoint in waypoint: ");
			str.AppendInt(randomPoint);
			debugprint(str);
			testWaypoint = m_Waypoints[randomPoint];
			break; //Break out of the while loop and go to return the testWaypoint
		}

		debugprint ("Finding waypoint looped");
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

