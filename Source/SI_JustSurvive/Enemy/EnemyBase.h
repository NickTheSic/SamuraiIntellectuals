// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnemyBase.generated.h"

/*
*	Change Log:
*			
*			2/10/2020: Nick - Added 2 functions. 1 to GetNewWaypoint and something to find the waypoint manager 
							- Made this inherit from ACharacter because that has a controller
*			2/9/2020: Nick - I added a Waypoint manager
*			2/8/2020: Nick - Initial Creation.  I am not sure how to do behaviour trees
*/

USTRUCT()
struct SI_JUSTSURVIVE_API FEnemyData
{
	GENERATED_BODY()

	//TODO: Look the Damage type stuff
	// I Think health is part of TakeDamage
	UPROPERTY(EditAnywhere, Category = "Config")
		float EnemyMaxHealth;

	UPROPERTY(EditAnywhere, Category = "Config")
		float CurrentHealth;

	//UPROPERTY(EditAnywhere, Category = "Config")
	//	Weapon or Inventory 
	
	UPROPERTY(EditAnywhere, Category = "Config")
		class USkeletalMesh* m_EnemyMesh;//TODO: Check if we need a Static or Skeletal and also set it here
};


UCLASS()
class SI_JUSTSURVIVE_API AEnemyBase : public ACharacter
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemyBase();

	//To be set when the enemy is spawned
	void SetWaypointManager(class AWaypointManager* wayMan);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Enemy Config" ) //Todo: Make this in protected somehow? With allow private access
		class USkeletalMeshComponent* m_EnemyMesh; //TODO: Maybe make this a Skeletal Mesh

	UPROPERTY(EditAnywhere, Category = "Enemy Config")
		FEnemyData EnemyData;

	void FindWaypointManager(); //If waypoint manager is null than we need to find it

	class AWaypointManager* m_WaypointManager = nullptr;

	//TODO: Add a Get new Waypoint function
	
	class AWaypoint* m_TargetWaypoint = nullptr;
	int m_CurrentWaypointGroup;

	void GetNewWaypoint();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
