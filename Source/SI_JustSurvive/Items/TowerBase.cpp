// Fill out your copyright notice in the Description page of Project Settings.


#include "TowerBase.h"
#include "Perception/PawnSensingComponent.h"
#include "SI_JustSurvive/SI_JustSurviveCharacter.h"
#include "DrawDebugHelpers.h"
#include "Engine/Engine.h"
#include "SI_JustSurvive/SI_JustSurviveProjectile.h"

ATowerBase::ATowerBase()
{
	TowerMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TowerMesh"));
	RootComponent = TowerMesh;

	m_PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComp"));
	m_PawnSensingComp->OnHearNoise.AddDynamic(this, &ATowerBase::OnNoiseHeard);

	FP_MuzzleLocation = CreateDefaultSubobject<USceneComponent>(TEXT("MuzzleLocation"));
	FP_MuzzleLocation->SetupAttachment(TowerMesh);
	FP_MuzzleLocation->SetRelativeLocation(FVector(0.2f, 48.4f, -10.6f));

	m_TowerData.m_FireRate = 1.0f;
}

void ATowerBase::BeginPlay()
{
	Super::BeginPlay();
	bCanShoot = false;
}

void ATowerBase::OnNoiseHeard(APawn * NoiseInstigator, const FVector & Location, float Volume)
{
	GetWorldTimerManager().SetTimer(SpawnProjectileTimer, this, &ATowerBase::ShootProjectile, m_TowerData.m_FireRate, true);

	FString message = TEXT("Saw Actor") + NoiseInstigator->GetName();
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, message);

	DrawDebugSphere(GetWorld(), Location, 32.0f, 12, FColor::Green, false, 10.0f);

	FVector Direction = Location - GetActorLocation();
	Direction.Normalize();

	FRotator NewLookAt = FRotationMatrix::MakeFromX(Direction).Rotator();
	NewLookAt.Pitch = 0.0f;
	NewLookAt.Roll = 0.0f;
	SetActorRotation(NewLookAt);

	//ShootProjectile();
}

void ATowerBase::ShootProjectile()
{
	//UWorld* const World = GetWorld();	
	//		
	//		const FRotator SpawnRotation = FP_MuzzleLocation->GetComponentRotation();
	//		const FVector SpawnLocation = FP_MuzzleLocation->GetComponentLocation();
	//		World->SpawnActor<ASI_JustSurviveProjectile>(ProjectileTemplate, SpawnLocation, SpawnRotation);

			if (ProjectileTemplate)
			{
				bCanShoot = true;
				UWorld* const World = GetWorld();
				if (World)
				{
					if (bCanShoot)
					{
						bCanShoot = false;
						FActorSpawnParameters SpawnParams;
						SpawnParams.Owner = this;
						SpawnParams.Instigator = Instigator;
						SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
						FVector SpawnLocation = FP_MuzzleLocation->GetComponentLocation();
						FRotator SpawnRotation = FP_MuzzleLocation->GetComponentRotation();

						ASI_JustSurviveProjectile* SpawnedProjectile = World->SpawnActor<ASI_JustSurviveProjectile>(ProjectileTemplate, SpawnLocation, SpawnRotation, SpawnParams);
						if (SpawnedProjectile)
						{
							SpawnedProjectile->SetLifeSpan(2.0f);
						}
					}
				}
			}
}

bool ATowerBase::Purchase(ASI_JustSurviveCharacter * player)
{
	return false;
}
