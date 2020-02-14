// Copyright 2020 Samurai Intellectuals


#include "TowerTerminal.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
ATowerTerminal::ATowerTerminal()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	m_BoxComponent = CreateDefaultSubobject<UBoxComponent>("BoxComponent"); 
	m_BoxComponent->SetCollisionProfileName("BlockAllDynamic"); 
	//TODO: This may be where we have to do trace things. 
	m_BoxComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics); 
	m_BoxComponent->SetNotifyRigidBodyCollision(true); 
	RootComponent = m_BoxComponent; 

	m_StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent"); 
	m_StaticMesh->SetCollisionProfileName("NoCollision"); 
	m_StaticMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision); 
	m_StaticMesh->SetupAttachment(RootComponent); 
}

// Called when the game starts or when spawned
void ATowerTerminal::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATowerTerminal::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

