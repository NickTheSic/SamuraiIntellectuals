// Copyright 2020 Samurai Intellectuals

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WaypointManager.generated.h"

/*
	Change Log:
			2/10/2020: Nick - Added onscreen dbg message
			2/9/2020: Nick - Initial Creation.  I feel like it would be nice to add a single pointer to all the groups
*/

UCLASS()
class SI_JUSTSURVIVE_API AWaypointManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWaypointManager();

	class AWaypoint* GetRandomWaypoint( int group);

	int GetWaypointGroupSize();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category="Waypoint")
		TArray<class AWaypointGroup*> m_WaypointGroups;
	
};
