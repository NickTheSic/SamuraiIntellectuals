// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnemyBase.generated.h"

/*
*	Change Log:
*			2/12/2020: Nick - Added 2 functions. 1 to GetNewWaypoint and something to find the waypoint manager 
							- Made this inherit from ACharacter because that has a controller
							- Added a distance that is editable and debug messages
							-The AI Follows the path
							-Removed the debug messages from the movement

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

	UPROPERTY(EditAnywhere, Category = "Config")
		float m_DistanceToPoint = 100;

	UPROPERTY(EditAnywhere, Category = "Config")
		float m_EnemySpeed = 50; //TODO: Actually set this up to work

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

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Noise")
		class UPawnNoiseEmitterComponent* NoiseEmitterComponent;

	UPROPERTY(EditAnywhere, Category = "Pawn Sensing")
		class UPawnSensingComponent* PawnSensing;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Enemy Config" ) //Todo: Make this in protected somehow? With allow private access
		class USkeletalMeshComponent* m_EnemyMesh; //TODO: Maybe make this a Skeletal Mesh

	UPROPERTY(EditAnywhere, Category = "Enemy Config")
		FEnemyData EnemyData;
	
	UPROPERTY(EditAnywhere, Category = "Health")
		float m_EnemyHP = 150.0f;

	UPROPERTY(EditAnywhere, Category = "Enemy Config")
		class ARoundManager* m_RoundManager; 

	UFUNCTION()
	void OnPawnSeen(APawn* pawn);
	APawn* TargetPawn;

	void FindWaypointManager(); //If waypoint manager is null than we need to find it

	class AWaypointManager* m_WaypointManager = nullptr;

	//TODO: Add a Get new Waypoint function
	
	class AWaypoint* m_TargetWaypoint = nullptr;
	int m_CurrentWaypointGroup;

	void GetNewWaypoint();

	void IncrementCurrentWaypointGroup(); 
	void DecrementCurrentWaypointGroup(); 


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
		void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	UFUNCTION()
		void TakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	void KillEnemy();
};
