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

	UPROPERTY(EditAnywhere, Category = "EnemyGroups")
		TArray<TSubclassOf<class AEnemyGroup>> m_EnemyGroups;

	int m_EnemyGroupIndex; 

	FTimerHandle EnemySpawnTimer;

	UPROPERTY(EditAnywhere, Category = "EnemyGroups")
		float m_EnemySpawnRate; 

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
