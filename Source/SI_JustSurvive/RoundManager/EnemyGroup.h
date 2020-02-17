// Copyright 2020 Samurai Intellectuals

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemyGroup.generated.h"

UCLASS()
class SI_JUSTSURVIVE_API AEnemyGroup : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemyGroup();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Enemies")
		TArray<TSubclassOf<class AEnemyBase>> m_Enemies; 

	int m_NextEnemyIndex = 0; 

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Enemies")
		class ASpawnPoint* m_SpawnLocation;

	FTimerHandle m_EnemySpawnTimer; 

	UPROPERTY(EditAnywhere, Category = "Enemies")
	float m_SpawnFrequency;

	float m_ApproxAllEnemiesDeployedTimer; 

	UWorld* m_World; 

    bool bSpawnedAllEnemies; 

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SpawnAllEnemies();

	void ClearSpawnEnemyTimer();

	void SpawnEnemy();

	int GetNumEnemies(); 

	float GetAllEnemiesDeployedTimer(); 

	void SetWorld(UWorld* world); 

	void SetSpawnLocation(ASpawnPoint* spawnPoint); 

    void ResetEnemyGroupData(); 

    bool WereAllEnemiesSpawned(); 
};
