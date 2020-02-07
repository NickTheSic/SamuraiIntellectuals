// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TargetPoint.h"
#include "Waypoint.generated.h"

/**
 * 
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
