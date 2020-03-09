// Copyright 2020 Samurai Intellectuals


#include "Round.h"

// Sets default values
ARound::ARound()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ARound::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ARound::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

