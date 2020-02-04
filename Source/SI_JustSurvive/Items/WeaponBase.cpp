// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponBase.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "SI_JustSurvive/SI_JustSurviveCharacter.h"
#include "SI_JustSurvive/SI_JustSurviveProjectile.h"

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
			const FRotator SpawnRotation = m_Character->GetControlRotation();

			if (m_Character != nullptr)
			{
				const FVector SpawnLocation = m_StaticMesh->GetSocketLocation("Muzzle");

				//Set Spawn Collision Handling Override
				FActorSpawnParameters ActorSpawnParams;
				ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

				//TODO: Change the projectile subclass of template to the actual projectile we plan to use.
				// spawn the projectile at the muzzle
				World->SpawnActor<ASI_JustSurviveProjectile>(ProjectileClass, SpawnLocation, SpawnRotation, ActorSpawnParams);
			}
		}
	}
}
