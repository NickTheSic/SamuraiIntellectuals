// Copyright 2020 Samurai Intellectuals

#pragma once

#include "CoreMinimal.h"
#include "ItemBase.h"
#include "PickupAmmo.generated.h"

/**
 * 
 */
UCLASS()
class SI_JUSTSURVIVE_API APickupAmmo : public AItemBase
{
	GENERATED_BODY()

public: 
	APickupAmmo(); 

	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	void AddAmmo(AActor* Weapon); 

protected:
	virtual void BeginPlay() override; 

	UPROPERTY(EditAnywhere, Category = "Ammo")
	int32 m_AmmoAmount; 
};
