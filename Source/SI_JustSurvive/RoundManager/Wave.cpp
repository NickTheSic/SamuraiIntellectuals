// Copyright 2020 Samurai Intellectuals


#include "Wave.h"
#include "EnemyGroup.h"
#include "TimerManager.h"
#include "Engine/World.h"
#include "SpawnPoint.h"

// Sets default values
AWave::AWave()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AWave::BeginPlay()
{
	Super::BeginPlay();
	
	 
}

// Called every frame
void AWave::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    if (bBeganSpawningEnemies)
    {
        if (m_EnemyGroups[m_EnemyGroupIndex]->GetDefaultObject<AEnemyGroup>()->WereAllEnemiesSpawned() == true)
        {
            bBeganSpawningEnemies = false; 
            m_EnemyGroupIndex++;
            SpawnNextEnemyGroup(); 
        }
    }
}

int AWave::GetNumEnemies()
{
	int numEnemies = 0;

	for (int i = 0; i < m_EnemyGroups.Num(); i++)
	{
		numEnemies += m_EnemyGroups[i]->GetDefaultObject<AEnemyGroup>()->GetNumEnemies(); 
	}
	return numEnemies;
}

void AWave::SpawnAllEnemyGroups()
{
    /*m_World->GetTimerManager().ClearTimer(m_EnemyGroupTimer);

	m_EnemyGroupTimerRate = m_EnemyGroups[m_EnemyGroupIndex]->GetDefaultObject<AEnemyGroup>()->GetAllEnemiesDeployedTimer();
	
	if (m_World)
	{
		m_World->GetTimerManager().SetTimer(m_EnemyGroupTimer, this, &AWave::SpawnNextEnemyGroup, m_EnemyGroupTimerRate + m_EnemyGroupTimerBuffer, true);
	}*/
    PrimaryActorTick.bCanEverTick = true;

    bBeganSpawningEnemies = true; 

    SpawnNextEnemyGroup(); 

}

void AWave::SpawnNextEnemyGroup()
{
	if (m_EnemyGroupIndex < m_EnemyGroups.Num())
	{
        bBeganSpawningEnemies = true; 
		m_EnemyGroups[m_EnemyGroupIndex]->GetDefaultObject<AEnemyGroup>()->SetWorld(m_World);
		m_EnemyGroups[m_EnemyGroupIndex]->GetDefaultObject<AEnemyGroup>()->SpawnAllEnemies();
		 
	}
}

void AWave::SetWorld(UWorld* world)
{
	m_World = world; 
}

void AWave::SetSpawnLocation(ASpawnPoint* spawnPoint)
{
	for (int i = 0; i < m_EnemyGroups.Num(); i++)
	{
		m_EnemyGroups[i]->GetDefaultObject<AEnemyGroup>()->SetSpawnLocation(spawnPoint); 
	}
}

void AWave::ResetWaveData()
{
    m_EnemyGroupIndex = 0;  

    if (m_EnemyGroups.Num() > 0)
    {
        for (int i = 0; i < m_EnemyGroups.Num(); i++)
        {
            m_EnemyGroups[i]->GetDefaultObject<AEnemyGroup>()->ResetEnemyGroupData();
        }
    }
}

