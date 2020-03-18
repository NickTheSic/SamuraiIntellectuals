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

    ResetRoundManagerData(); 
 
    if (GetLocalRole() != ROLE_Authority)
        return;

	if (m_Waves.Num() > 0)
	{
		if (GetWorld() != nullptr)
		{
            SpawnNextWave(); 
		}
	}
	
}

// Called every frame
void ARoundManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    if (GetLocalRole() != ROLE_Authority)
        return;

    if (m_WaveIndex < m_Waves.Num())
    {
        m_Waves[m_WaveIndex]->GetDefaultObject<AWave>()->Tick(DeltaTime);
    }
    
}

void ARoundManager::RemoveEnemy()
{
    if (GetLocalRole() != ROLE_Authority)
        return;

	if (m_ActiveEnemiesInCurrentWave > 0)
	{
		m_ActiveEnemiesInCurrentWave--; 

        GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, FString::Printf(TEXT("CurrentActiveEnemies: %d"), m_ActiveEnemiesInCurrentWave));
	}
	if (m_ActiveEnemiesInCurrentWave == 0)
	{
		m_WaveIndex++; 

        GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, FString::Printf(TEXT("WaveSwitchedTo: %d"), m_WaveIndex));

		if (m_WaveIndex < m_Waves.Num())
		{
            GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, TEXT("Spawning Next Wave"));

            GetWorld()->GetTimerManager().ClearTimer(m_WaveSpawnTimer); 
            GetWorld()->GetTimerManager().SetTimer(m_WaveSpawnTimer, this, &ARoundManager::SpawnNextWave, m_WaveSpawnRate, false); 
            //SpawnNextWave(); 
		}
	}
}

int ARoundManager::GetNumEnemies()
{
	return m_Waves[m_WaveIndex]->GetDefaultObject<AWave>()->GetNumEnemies();
}

void ARoundManager::ResetRoundManagerData()
{
    m_WaveIndex = 0; 

    if (m_Waves.Num() > 0)
    {
        for (int i = 0; i < m_Waves.Num(); i++)
        {
            m_Waves[i]->GetDefaultObject<AWave>()->ResetWaveData();
        }
    }

}

void ARoundManager::SpawnNextWave()
{
    if (GetLocalRole() != ROLE_Authority)
        return;

    m_ActiveEnemiesInCurrentWave = GetNumEnemies();

    m_Waves[m_WaveIndex]->GetDefaultObject<AWave>()->SetWorld(GetWorld());

    if (m_SpawnLocation != nullptr)
        m_Waves[m_WaveIndex]->GetDefaultObject<AWave>()->SetSpawnLocation(m_SpawnLocation);

    m_Waves[m_WaveIndex]->GetDefaultObject<AWave>()->SpawnAllEnemyGroups();
}

