// Copyright 2020 Samurai Intellectuals

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WaypointGroup.generated.h"

/*
*	Change Log:
*			
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

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
