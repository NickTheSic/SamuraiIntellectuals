// Copyright 2020 Samurai Intellectuals


#include "GeneratorBase.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"

// Sets default values
AGeneratorBase::AGeneratorBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//ADDED START

	GeneratorRoot = CreateDefaultSubobject<USceneComponent>("Actor Root");
	RootComponent = GeneratorRoot;

	GeneratorBody = CreateDefaultSubobject<UStaticMeshComponent>("Generator Mesh");


}

// Called when the game starts or when spawned
void AGeneratorBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void AGeneratorBase::TakeAnyDamage(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageInflictor)
{
}

// Called every frame
void AGeneratorBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}

