// Copyright 2020 Samurai Intellectuals

#pragma once

#include "CoreMinimal.h"
#include "Engine/TargetPoint.h"
#include "Waypoint.generated.h"

/**
 * Change Log:
				2/12/2020: Nick - Added Networking notes
 *				Initial Creation - Nick
 */
UCLASS()
class SI_JUSTSURVIVE_API AWaypoint : public ATargetPoint
{
	GENERATED_BODY()
	
public:
	bool GetIsWaypointTaken();

	//TODO: Waypoint may need to be networked?
	void SetIsWaypointTaken(bool isTaken);

protected:
	//TODO: Waypoints - May need to be replicated? 
	//It is only changed on the server, and then again the AI should only do logic on the server too
	bool bIsWaypointTaken = false; //They always start the game not taken
};
