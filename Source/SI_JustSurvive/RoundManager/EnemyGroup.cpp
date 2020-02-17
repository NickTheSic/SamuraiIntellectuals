// Copyright 2020 Samurai Intellectuals


#include "EnemyGroup.h"
#include <TimerManager.h>
#include "../Enemy/EnemyBase.h"
#include "Engine/World.h"
#include "SpawnPoint.h"

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

	//SpawnAllEnemies(); 
	
}

// Called every frame
void AEnemyGroup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnemyGroup::SpawnAllEnemies()
{
	m_World->GetTimerManager().SetTimer(m_EnemySpawnTimer, this, &AEnemyGroup::SpawnEnemy, m_SpawnFrequency, true); 
}

void AEnemyGroup::ClearSpawnEnemyTimer()
{
	m_World->GetTimerManager().ClearTimer(m_EnemySpawnTimer); 
}

void AEnemyGroup::SpawnEnemy()
{
	if (m_NextEnemyIndex < m_Enemies.Num())
	{
		if (m_Enemies[m_NextEnemyIndex] != nullptr)
		{
			//UWorld* const World = GetWorld();

			if (m_World != nullptr)
			{
				if (m_SpawnLocation != nullptr)
				{
					//TODO: Find a way to use the m_SpawnLocation Location and Rotation
					//const FVector SpawnLocation = FVector(0.0f, 0.0f, 200.0f); 
					//const FVector SpawnLocation = Cast<AActor>(m_SpawnLocation->GetDefaultObject())->GetActorLocation();
					const FVector SpawnLocation = m_SpawnLocation->GetActorLocation();
					const FRotator SpawnRotation = FRotator(0.0f, 0.0f, 0.0f);

					FActorSpawnParameters ActorSpawnParams;
					ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

					AEnemyBase* enemy = Cast<AEnemyBase>(m_World->SpawnActor<AEnemyBase>(m_Enemies[m_NextEnemyIndex], SpawnLocation, SpawnRotation, ActorSpawnParams));

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

int AEnemyGroup::GetNumEnemies()
{
	return m_Enemies.Num();
}

float AEnemyGroup::GetAllEnemiesDeployedTimer()
{
	check(m_Enemies.Num() != 0); 

	m_ApproxAllEnemiesDeployedTimer = m_SpawnFrequency * m_Enemies.Num(); 

	return m_ApproxAllEnemiesDeployedTimer;
}

void AEnemyGroup::SetWorld(UWorld* world)
{
	m_World = world; 
}

void AEnemyGroup::SetSpawnLocation(ASpawnPoint* spawnPoint)
{
	m_SpawnLocation = spawnPoint; 
}

