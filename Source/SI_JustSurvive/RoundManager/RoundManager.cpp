// Copyright 2020 Samurai Intellectuals


#include "RoundManager.h"
#include "Wave.h"
#include "TimerManager.h"

// Sets default values
ARoundManager::ARoundManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ARoundManager::BeginPlay()
{
	Super::BeginPlay();

	m_WaveIndex = 0; 
 
	if (m_Waves.Num() > 0)
	{
		if (GetWorld() != nullptr)
		{
			m_ActiveEnemies = GetNumEnemies(); 

			m_Waves[m_WaveIndex]->GetDefaultObject<AWave>()->SetWorld(GetWorld());

			if (m_SpawnLocation !=nullptr)
				m_Waves[m_WaveIndex]->GetDefaultObject<AWave>()->SetSpawnLocation(m_SpawnLocation); 

			m_Waves[m_WaveIndex]->GetDefaultObject<AWave>()->SpawnAllEnemyGroups();		
		}
	}
	
}

// Called every frame
void ARoundManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ARoundManager::RemoveEnemy()
{
	if (m_ActiveEnemies > 0)
	{
		m_ActiveEnemies--; 
	}
	if (m_ActiveEnemies == 0)
	{
		m_WaveIndex++; 

		if (m_WaveIndex < m_Waves.Num())
		{
			m_ActiveEnemies = GetNumEnemies(); 
		}
	}
}

int ARoundManager::GetNumEnemies()
{
	return m_Waves[m_WaveIndex]->GetDefaultObject<AWave>()->GetNumEnemies();
}

