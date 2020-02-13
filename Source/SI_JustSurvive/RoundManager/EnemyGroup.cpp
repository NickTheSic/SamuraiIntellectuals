// Copyright 2020 Samurai Intellectuals


#include "EnemyGroup.h"
#include <TimerManager.h>
#include "../Enemy/EnemyBase.h"

// Sets default values
AEnemyGroup::AEnemyGroup()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AEnemyGroup::BeginPlay()
{
	Super::BeginPlay();

	SpawnAllEnemies(); 
	
}

// Called every frame
void AEnemyGroup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnemyGroup::SpawnAllEnemies()
{
	GetWorld()->GetTimerManager().SetTimer(m_EnemySpawnTimer, this, &AEnemyGroup::SpawnEnemy, m_SpawnFrequency, true); 
}

void AEnemyGroup::ClearSpawnEnemyTimer()
{
	GetWorld()->GetTimerManager().ClearTimer(m_EnemySpawnTimer); 
}

void AEnemyGroup::SpawnEnemy()
{
	if (m_NextEnemyIndex < m_Enemies.Num())
	{
		if (m_Enemies[m_NextEnemyIndex] != nullptr)
		{
			UWorld* const World = GetWorld();

			if (World != nullptr)
			{
				if (m_SpawnLocation != nullptr)
				{
					//TODO: Find a way to use the m_SpawnLocation Location and Rotation
					const FVector SpawnLocation = FVector(0.0f, 0.0f, 200.0f); 
					const FRotator SpawnRotation = FRotator(0.0f, 0.0f, 0.0f);

					FActorSpawnParameters ActorSpawnParams;
					ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

					World->SpawnActor<AEnemyBase>(m_Enemies[m_NextEnemyIndex], SpawnLocation, SpawnRotation, ActorSpawnParams);

					m_NextEnemyIndex++;
				}
			}
		}
	}

	if (m_NextEnemyIndex >= m_Enemies.Num())
	{
		ClearSpawnEnemyTimer(); 
	}
}

