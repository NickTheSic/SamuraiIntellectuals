// Copyright 2020 Samurai Intellectuals


#include "PickupAmmo.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "../Components/InventoryComponent.h"
#include "../SI_JustSurviveCharacter.h"
#include "../Items/WeaponBase.h"


APickupAmmo::APickupAmmo()
{
	m_BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent")); 
	m_BoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics); 
	m_BoxComponent->SetCollisionProfileName("BlockAllDynamic"); 
	m_BoxComponent->SetNotifyRigidBodyCollision(true); 
	m_BoxComponent->OnComponentHit.AddDynamic(this, &APickupAmmo::OnHit); 
	RootComponent = m_BoxComponent; 

	m_StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	m_StaticMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision); 
	m_StaticMesh->SetCollisionProfileName("NoCollision"); 
	m_StaticMesh->SetupAttachment(RootComponent); 

	//SetReplicates(true); 
	//SetReplicateMovement(true); 
}

void APickupAmmo::BeginPlay()
{

}

void APickupAmmo::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor != nullptr)
	{
		if (Cast<ASI_JustSurviveCharacter>(OtherActor))
		{
			ASI_JustSurviveCharacter* tempActor = Cast<ASI_JustSurviveCharacter>(OtherActor);

			if (tempActor->GetInventoryComponent()->GetCurrentWeapon() != nullptr)
			{
				
				tempActor->GetInventoryComponent()->GetCurrentWeapon()->AddAmmo(m_AmmoAmount);
				m_BoxComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision); 
				m_BoxComponent->SetCollisionProfileName("NoCollision"); 
				m_BoxComponent->SetNotifyRigidBodyCollision(false); 
				m_StaticMesh->SetVisibility(false); 
				Destroy(); 
			}

		}
	}
}

void APickupAmmo::AddAmmo(AActor* Weapon)
{

}


