// Copyright 2020 Samurai Intellectuals

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Wave.generated.h"

UCLASS()
class SI_JUSTSURVIVE_API AWave : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWave();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	class AEnemyGroup* m_CurrentEnemyGroup; 

	int m_EnemyGroupIndex = 0; 

	UPROPERTY(EditAnywhere, Category = "EnemyGroups")
		float m_EnemySpawnRate; 

	UPROPERTY(EditAnywhere, Category = "EnemyGroups")
		TArray<TSubclassOf<AEnemyGroup>> m_EnemyGroups;

	//FTimerManager m_TimerManager; 

	FTimerHandle m_EnemyGroupTimer;

	UWorld* m_World; 

	float m_EnemyGroupTimerRate; 

	UPROPERTY(EditAnywhere, Category = "EnemyGroups")
		float m_EnemyGroupTimerBuffer = 0.0f; 

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	int GetNumEnemies(); 

	void SpawnAllEnemyGroups();

	void SpawnNextEnemyGroup();

	void SetWorld(UWorld* world); 

	void SetSpawnLocation(class ASpawnPoint* spawnPoint);

};
