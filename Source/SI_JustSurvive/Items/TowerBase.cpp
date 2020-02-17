// Fill out your copyright notice in the Description page of Project Settings.


#include "TowerBase.h"
#include "Perception/PawnSensingComponent.h"
#include "SI_JustSurvive/SI_JustSurviveCharacter.h"
#include "DrawDebugHelpers.h"
#include "Engine/Engine.h"
#include "SI_JustSurvive/SI_JustSurviveProjectile.h"
#include "..//Player/SI_PlayerController.h"
#include "Components/StaticMeshComponent.h"

ATowerBase::ATowerBase()
{
	m_TowerMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TowerMesh"));
	RootComponent = m_TowerMesh;

	m_PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComp"));
	m_PawnSensingComp->OnHearNoise.AddDynamic(this, &ATowerBase::OnNoiseHeard);

	m_MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleLocation"));
	m_MuzzleLocation->SetupAttachment(m_TowerMesh);
	m_MuzzleLocation->SetRelativeLocation(FVector(0.2f, 48.4f, -10.6f));

	m_TowerData.m_FireRate = 1.5f;
	
}

void ATowerBase::BeginPlay()
{
	Super::BeginPlay();
	bCanShoot = false;

    if (ItemShopData.m_StaticMesh != nullptr)
    {
        m_TowerMesh->SetStaticMesh(ItemShopData.m_StaticMesh);
    }
}

void ATowerBase::OnNoiseHeard(APawn * NoiseInstigator, const FVector & Location, float Volume)
{
	//GetWorldTimerManager().SetTimer(SpawnProjectileTimer, this, &ATowerBase::ShootProjectile, m_TowerData.m_FireRate, true);

	FString message = TEXT("Saw Actor") + NoiseInstigator->GetName();
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, message);

	DrawDebugSphere(GetWorld(), Location, 32.0f, 12, FColor::Green, false, 10.0f);

	FVector Direction = Location - GetActorLocation();
	Direction.Normalize();

	FRotator NewLookAt = FRotationMatrix::MakeFromX(Direction).Rotator();
	NewLookAt.Pitch = 0.0f;
	NewLookAt.Roll = 0.0f;
	SetActorRotation(NewLookAt);
	
	ShootProjectile();
	bCanShoot = false;
}

void ATowerBase::ShootProjectile()
{
	if (m_ProjectileTemplate)
	{
		bCanShoot = true;
		UWorld* const World = GetWorld();
		if (World)
		{
			if (bCanShoot)
			{
				FActorSpawnParameters SpawnParams;
				SpawnParams.Owner = this;
				SpawnParams.Instigator = GetInstigator();
				SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
				FVector SpawnLocation = m_MuzzleLocation->GetComponentLocation();
				FRotator SpawnRotation = m_MuzzleLocation->GetComponentRotation();

				ASI_JustSurviveProjectile* SpawnedProjectile = World->SpawnActor<ASI_JustSurviveProjectile>(m_ProjectileTemplate, SpawnLocation, SpawnRotation, SpawnParams);
				if (SpawnedProjectile)
				{
					SpawnedProjectile->SetLifeSpan(2.0f);
				}
			}
		}
	}
	bCanShoot = false;
}

bool ATowerBase::Purchase(ASI_PlayerController* player)
{
	bool bCanPurchase = AItemBase::Purchase(player);
	if (bCanPurchase)
	{
		//Purchase
		return true;
	}
	else
	{
		//Do nothing
		return false;
	}
	return false;
}

void ATowerBase::InitializeTower()
{
	//TODO: Check and setup what else needs to be initialized
	if (ItemShopData.m_StaticMesh != nullptr)
	{
		m_TowerMesh->SetStaticMesh(ItemShopData.m_StaticMesh);
	}
}
