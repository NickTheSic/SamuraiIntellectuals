// Copyright 2020 Samurai Intellectuals

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RoundManager.generated.h"

UCLASS()
class SI_JUSTSURVIVE_API ARoundManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARoundManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Waves")
		TArray<TSubclassOf< class AWave>> m_Waves;

	AWave* m_CurrentWave; 

	int m_WaveIndex = 0; 

	int m_ActiveEnemiesInCurrentWave; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enemies")
		class ASpawnPoint* m_SpawnLocation;

    FTimerHandle m_WaveSpawnTimer; 

    UPROPERTY(EditAnywhere, Category = "Waves")
        float m_WaveSpawnRate = 10.0f; 

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void RemoveEnemy();

	int GetNumEnemies(); 

    void ResetRoundManagerData(); 

    void SpawnNextWave(); 
};
