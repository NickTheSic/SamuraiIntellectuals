// Fill out your copyright notice in the Description page of Project Settings.


#include "TowerBase.h"
#include "Perception/PawnSensingComponent.h"
#include "SI_JustSurvive/SI_JustSurviveCharacter.h"
#include "DrawDebugHelpers.h"
#include "Engine/Engine.h"

ATowerBase::ATowerBase()
{
	TowerMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TowerMesh"));
	RootComponent = TowerMesh;

	m_PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComp"));
	m_PawnSensingComp->OnHearNoise.AddDynamic(this, &ATowerBase::OnNoiseHeard);
}

void ATowerBase::OnNoiseHeard(APawn * NoiseInstigator, const FVector & Location, float Volume)
{
	FString message = TEXT("Saw Actor") + NoiseInstigator->GetName();
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, message);

	DrawDebugSphere(GetWorld(), Location, 32.0f, 12, FColor::Green, false, 10.0f);

	FVector Direction = Location - GetActorLocation();
	Direction.Normalize();

	FRotator NewLookAt = FRotationMatrix::MakeFromX(Direction).Rotator();
	NewLookAt.Pitch = 0.0f;
	NewLookAt.Roll = 0.0f;
	SetActorRotation(NewLookAt);
}

void ATowerBase::ShootProjectile()
{
}

bool ATowerBase::Purchase(ASI_JustSurviveCharacter * player)
{
	return false;
}
