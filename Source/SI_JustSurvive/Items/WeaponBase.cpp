// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponBase.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "SI_JustSurvive/SI_JustSurviveCharacter.h"
#include "SI_JustSurvive/SI_JustSurviveProjectile.h"
#include <TimerManager.h>

AWeaponBase::AWeaponBase()
{
	PrimaryActorTick.bCanEverTick = true;

	m_BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	m_BoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics); 
	m_BoxComponent->SetCollisionProfileName("BlockAllDynamic"); 
	m_BoxComponent->SetNotifyRigidBodyCollision(true); 
	RootComponent = m_BoxComponent; 

	m_StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh")); 
	m_StaticMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision); 
	m_StaticMesh->SetupAttachment(RootComponent); 
}

void AWeaponBase::BeginPlay()
{
	Super::BeginPlay(); 

	m_TotalAmmo = m_ClipSize * 2; 
}

void AWeaponBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime); 
}

void AWeaponBase::PickUp(ASI_JustSurviveCharacter* character)
{
	m_Character = character; 

	AttachToComponent(Cast<USceneComponent>(character->GetMesh1P()), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint"));
	Equip();
}

void AWeaponBase::Equip()
{
	SetActorEnableCollision(false); 
	SetActorHiddenInGame(false); 
	bIsActive = true; 
}

void AWeaponBase::Unequip()
{
	SetActorHiddenInGame(true); 
	bIsActive = false; 
}

void AWeaponBase::Shoot()
{
	if (ProjectileClass != NULL)
	{
		UWorld* const World = GetWorld(); 
		if (World != NULL)
		{
			if (m_GunData.m_AmmoInClip > 0)
			{
				const FRotator SpawnRotation = m_Character->GetControlRotation();

				if (m_Character != nullptr)
				{
					const FVector SpawnLocation = m_StaticMesh->GetSocketLocation("Muzzle");

					//Set Spawn Collision Handling Override
					FActorSpawnParameters ActorSpawnParams;
					ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

					//TODO: Change the projectile subclass of template to the actual projectile we plan to use.
					// spawn the projectile at the muzzle
					World->SpawnActor<ASI_JustSurviveProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);

					m_GunData.m_AmmoInClip--; 
				}
			}
		}
	}
}

void AWeaponBase::PullTrigger()
{
	FTimerManager& Timer = GetWorldTimerManager();
	Timer.SetTimer(m_FireRateTimer, this, &AWeaponBase::Shoot, m_GunData.m_FireRate, true);
}

void AWeaponBase::ReleaseTrigger()
{
	GetWorld()->GetTimerManager().ClearTimer(m_FireRateTimer);
}

void AWeaponBase::Reload()
{
	int ammocheck = m_GunData.m_TotalAmmo - (m_GunData.m_ClipSize - m_GunData.m_AmmoInClip);

	if (ammocheck > 0)
	{
		m_GunData.m_TotalAmmo -= m_GunData.m_ClipSize - m_GunData.m_AmmoInClip;
		m_GunData.m_AmmoInClip = m_GunData.m_ClipSize;
	}
	else if (ammocheck < 0 )
	{
		if ((m_GunData.m_AmmoInClip + m_GunData.m_TotalAmmo) <= m_GunData.m_ClipSize)
		{
			m_GunData.m_AmmoInClip += m_GunData.m_TotalAmmo;
			m_GunData.m_TotalAmmo = 0;
		}
	}
	else
	{
		m_GunData.m_AmmoInClip = m_GunData.m_TotalAmmo;
		m_GunData.m_TotalAmmo = 0;
	}
}
