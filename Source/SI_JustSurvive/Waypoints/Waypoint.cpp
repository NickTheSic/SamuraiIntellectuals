// Fill out your copyright notice in the Description page of Project Settings.


#include "Waypoint.h"

bool AWaypoint::GetIsWaypointTaken()
{
	return bIsWaypointTaken;
}

//Make sure to always call this function twice.  
//Once for taken and once for when it is free
void AWaypoint::SetIsWaypointTaken(bool isTaken)
{
	bIsWaypointTaken = isTaken;
}