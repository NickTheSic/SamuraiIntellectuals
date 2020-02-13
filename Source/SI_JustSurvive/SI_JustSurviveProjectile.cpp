// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "SI_JustSurviveProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "GameFramework/DamageType.h"


ASI_JustSurviveProjectile::ASI_JustSurviveProjectile() 
{
	// Use a sphere as a simple collision representation
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
	CollisionComp->OnComponentHit.AddDynamic(this, &ASI_JustSurviveProjectile::OnHit);		// set up a notification for when this component hits something blocking

	// Players can't walk on it
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;

	// Set as root component
	RootComponent = CollisionComp;

	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;

	// Die after 3 seconds by default
	InitialLifeSpan = 3.0f;
}

void ASI_JustSurviveProjectile::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// Only add impulse and destroy projectile if we hit a physics
	if ((OtherActor != NULL) && (OtherActor != this) && (OtherComp != NULL) && OtherComp->IsSimulatingPhysics())
	{
		//Comment out for now and decide if needed later
		OtherComp->AddImpulseAtLocation(GetVelocity() * 100.0f, GetActorLocation());

		//CHANGES START
		EPhysicalSurface SurfaceType = SurfaceType_Default;
		FBodyInstance* BodyInstance = OtherComp->GetBodyInstance();
		UPhysicalMaterial* PhysicsMat = BodyInstance->GetSimplePhysicalMaterial();

		SurfaceType = UPhysicalMaterial::DetermineSurfaceType(PhysicsMat);

		float m_DamageAmount = 50.0f;

		AController* MyInstigatorsController = nullptr;
		APawn* MyInstigator = this->Instigator;		

		if (MyInstigator)
			MyInstigatorsController = MyInstigator->GetController();

		TSubclassOf<UDamageType> const ValidDamageTypeClass = TSubclassOf<UDamageType>(UDamageType::StaticClass());
		FDamageEvent DamageEvent(ValidDamageTypeClass);

		OtherActor->TakeDamage(m_DamageAmount, DamageEvent, MyInstigatorsController, this);
		//CHANGES END

		Destroy();
	}
}