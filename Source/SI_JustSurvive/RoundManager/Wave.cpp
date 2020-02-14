// Copyright 2020 Samurai Intellectuals


#include "Wave.h"

// Sets default values
AWave::AWave()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AWave::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWave::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

