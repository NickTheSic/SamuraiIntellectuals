// Copyright 2020 Samurai Intellectuals


#include "GeneratorBase.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"
#include "Engine/Engine.h"
#include "SI_JustSurvive/Player/SI_PlayerController.h"


// Sets default values
AGeneratorBase::AGeneratorBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	m_ActorRoot = CreateDefaultSubobject<USceneComponent>("Actor Root");
	RootComponent = m_ActorRoot;

	m_GenMesh = CreateDefaultSubobject<UStaticMeshComponent>("Generator Mesh");
	m_GenMesh->SetCollisionProfileName("BlockAllDynamic");
	m_GenMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	m_GenMesh->SetSimulatePhysics(false);
	m_GenMesh->SetNotifyRigidBodyCollision(true);
	m_GenMesh->SetupAttachment(RootComponent);

	SetCanBeDamaged(true);	

	OnTakeAnyDamage.AddDynamic(this, &AGeneratorBase::TakeAnyDamage);

}

// Called when the game starts or when spawned
void AGeneratorBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void AGeneratorBase::TakeAnyDamage(AActor* DamagedActor, float Damage, const class UDamageType* DamageType,	class AController* InstigatedBy, AActor* DamageCauser)
{
	float m_DamageAmount = 0;
	m_DamageAmount = Damage;

	m_HP -= m_DamageAmount;
	GEngine->AddOnScreenDebugMessage(-1, 3, FColor::Green, "Generator under attack! " + FString::FromInt(Damage) + " damage taken, " + FString::FromInt(m_HP) + "HP left");

	//TODO:: Implement "Game Over Screen" if hp reaches zero
	if (m_HP <= 0)
	{
		Destroy();
		ASI_PlayerController* pController = Cast <ASI_PlayerController>(GetWorld()->GetFirstPlayerController());
		pController->InitiateGameOver();
	}

}

// Called every frame
void AGeneratorBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

