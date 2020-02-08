// Copyright 2020 Samurai Intellectuals

#pragma once

#include "CoreMinimal.h"
#include "Engine/TargetPoint.h"
#include "Waypoint.generated.h"

/**
 * Change Log:
 *				Initial Creation - Nick
 */
UCLASS()
class SI_JUSTSURVIVE_API AWaypoint : public ATargetPoint
{
	GENERATED_BODY()
	
public:
	bool GetIsWaypointTaken();
	void SetIsWaypointTaken(bool isTaken);

protected:
	bool bIsWaypointTaken = false; //They always start the game not taken
};
