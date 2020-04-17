// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UObject/ConstructorHelpers.h"
#include "SI_JustSurviveProjectile.generated.h"

UCLASS(config=Game)
class ASI_JustSurviveProjectile : public AActor
{
	GENERATED_BODY()

	/** Sphere collision component */
	UPROPERTY(VisibleDefaultsOnly, Category=Projectile)
	class USphereComponent* CollisionComp;

	/** Projectile movement component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	class UProjectileMovementComponent* ProjectileMovement;


public:
	ASI_JustSurviveProjectile();

	FString GetDamageType() { return m_DamageType; }

	/** called when projectile hits something */
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	/** Returns CollisionComp subobject **/
	FORCEINLINE class USphereComponent* GetCollisionComp() const { return CollisionComp; }
	/** Returns ProjectileMovement subobject **/
	FORCEINLINE class UProjectileMovementComponent* GetProjectileMovement() const { return ProjectileMovement; }

protected:
	UPROPERTY(EditAnywhere, Category = "Damage Type")
		FString m_DamageType;

	UPROPERTY (EditAnywhere, Category = "Damage")
		float m_DamageAmount = 50.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Particle Effects")
		UParticleSystem* m_ParticleEffect;

};

