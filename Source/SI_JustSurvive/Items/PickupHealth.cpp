// Copyright 2020 Samurai Intellectuals


#include "PickupHealth.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "../SI_JustSurviveCharacter.h"

APickupHealth::APickupHealth()
{
	m_BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	m_BoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics); 
	m_BoxComponent->SetCollisionProfileName("BlockAllDynamic"); 
	m_BoxComponent->SetNotifyRigidBodyCollision(true);
	m_BoxComponent->OnComponentHit.AddDynamic(this, &APickupHealth::OnHit); 
	RootComponent = m_BoxComponent; 

	m_StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	m_StaticMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision); 
	m_StaticMesh->SetupAttachment(RootComponent); 



	SetReplicates(true); 
	SetReplicateMovement(true); 
}

void APickupHealth::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (OtherActor != nullptr)
	{
		if (Cast<ASI_JustSurviveCharacter>(OtherActor))
		{
			AddHealth(OtherActor); 
			Destroy(); 
		}
	}
}

void APickupHealth::AddHealth(AActor* Player)
{

}