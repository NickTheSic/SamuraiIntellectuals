// Copyright 2020 Samurai Intellectuals

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WaypointGroup.generated.h"

/*
*	Change Log:
			2/12/2020: Nick - Added some debug stuff. Removed the tick function
							- Removed some debug stuff to make it cleaner code
*			2/10/2020: Nick - Added on screen dbg message
							- Fixed some logical issues
*			Initial creation - Nick
*/

UCLASS()
class SI_JUSTSURVIVE_API AWaypointGroup : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWaypointGroup();

	UPROPERTY(EditAnywhere, Category = "Waypoints")
		TArray<class AWaypoint*> m_Waypoints;

	class AWaypoint* GetRandomWaypoint();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};
